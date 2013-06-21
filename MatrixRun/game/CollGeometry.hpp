#ifndef COLLGEOMETRY_HPP_INCLUDED
#define COLLGEOMETRY_HPP_INCLUDED

#include <irrlicht.h>

#include "World.hpp"

namespace game
{

	namespace coll
	{

	//====================================
		/**
		@brief Represents a collision, with its position, triangle and associated node
		**/
		class Collision
		{
		public:
			Collision()
			{
				node = 0;
			}

			Collision(irr::scene::ISceneNode* node_, irr::core::triangle3df& triangle_, irr::core::vector3df& position_)
			{
				node = node_;
				triangle = triangle_;
				position = position_;
			}

			bool GetIsValid()
			{
				if(node>0)return true;
				return false;
			}

			irr::scene::ISceneNode* node;
			irr::core::triangle3df triangle;
			irr::core::vector3df position;
		};

	//====================================
		/**
		@brief Abstract class to define a standart collision generator such as point or line
		**/
		class CollGeometry
		{
		public:
			virtual Collision* GetCollision(irr::scene::ISceneCollisionManager* oCollM, irr::scene::ISceneNode* nodeAttachedNode, int nFlags)=0;

		protected:
			Collision m_Collision;
		};

	//====================================
		/**
		@brief A point that generates collisons
		**/
		class CollPoint : public CollGeometry
		{
		public:
			/**
			@param bDrawLines true pour que le calcul se fasse sur toute la ligne formée par l'ancienne position de calcul et la nouvelle
			**/
			CollPoint(irr::scene::ISceneNode* nodeAttachedNode, const irr::core::vector3df& vPos, bool bDrawLines=true)
			{
				m_vPosRelNode.set(vPos);
				m_bDrawLines = bDrawLines;
				m_vLastPosAbs = 0;
			}
			//=======
			~CollPoint()
			{
				if(m_vLastPosAbs !=0)delete m_vLastPosAbs;
			}
			//=======
			Collision* GetCollision(irr::scene::ISceneCollisionManager* oCollM, irr::scene::ISceneNode* nodeAttachedNode, int nFlags)
			{
				irr::core::vector3df m_vCurrPosAbs(nodeAttachedNode->getAbsolutePosition());
				irr::core::line3d<f32> Ray;
				if(m_bDrawLines)
				{
					if(m_vLastPosAbs == 0)
					{
						m_vLastPosAbs = new irr::core::vector3df(m_vCurrPosAbs+m_vPosRelNode);
					}
					Ray.setLine(*m_vLastPosAbs, m_vCurrPosAbs+m_vPosRelNode);
					m_vLastPosAbs->set(m_vCurrPosAbs+m_vPosRelNode);
				}
				else
				{
					Ray.setLine(m_vCurrPosAbs, m_vCurrPosAbs);
				}

				//Specific collision with tunnel
				if(nFlags & game::tunnel_border)
				{
					if(!game::World::GetInstance()->GetTunnel()->GetIsInTunnel(m_vCurrPosAbs, &m_Collision.node, &(m_Collision.position)))
					{
						return &m_Collision;
					}

				}

				m_Collision.node = oCollM->getSceneNodeAndCollisionPointFromRay(Ray, m_Collision.position, m_Collision.triangle, nFlags);
				if(m_Collision.GetIsValid())
					return &m_Collision;
				return 0;
			}

		private:
			irr::core::vector3df m_vPosRelNode;
			bool m_bDrawLines;
			irr::core::vector3df* m_vLastPosAbs;
		};
	//====================================
		/**
		@brief A line that generates collisons
		**/
		class CollLine : public CollGeometry
		{
		public:
			CollLine(irr::scene::ISceneNode* nodeAttachedNode, const irr::core::line3df& Line, bool bDrawSquares=false, float fPrecision=0.0)
			{
				m_vLineRelNode = Line;
				m_bDrawSquares = bDrawSquares;
				m_fPrecision = fPrecision;
				m_vLastPosAbs = 0;
			}
			//=======
			~CollLine()
			{
				if(m_vLastPosAbs !=0)delete m_vLastPosAbs;
			}
			//=======
			const irr::core::line3df& GetLine()const
			{
				return m_vLineRelNode;
			}
			//=======
			Collision* GetCollision(irr::scene::ISceneCollisionManager* oCollM, irr::scene::ISceneNode* nodeAttachedNode, int nFlags)
			{
				irr::core::vector3df m_vCurrPosAbs(nodeAttachedNode->getAbsolutePosition());
				if(m_bDrawSquares)
				{/*
					if(m_vLastPosAbs == 0)
					{
						m_vLastPosAbs = new irr::core::vector3df(m_vCurrPosAbs+m_vPosRelNode);
					}
					irr::core::line3df lineDelta(*m_vLastPosAbs, m_vCurrPosAbs);
					float fLength = lineDelta.getLength();
					for(float f=0.0 ; f<fLength ; f+=m_fPrecision)
					{
						irr::core::line3d<f32> Ray(lineDelta.start+m_vLineRelNode.start, lineDelta.start+m_vLineRelNode.end);//FIX HERE
						m_Collision.node = oCollM->getSceneNodeAndCollisionPointFromRay(Ray, m_Collision.position, m_Collision.triangle, nFlags);

						if(m_Collision.GetIsValid())
							break;
					}

					m_vLastPosAbs->set(m_vCurrPosAbs+m_vLineRelNode.start);*/
				}
				else
				{
					irr::core::line3d<f32> Ray(m_vCurrPosAbs+m_vLineRelNode.start, m_vCurrPosAbs+m_vLineRelNode.end);
					m_Collision.node = oCollM->getSceneNodeAndCollisionPointFromRay(Ray, m_Collision.position, m_Collision.triangle, nFlags);
				}



				return &m_Collision;
			}

		private:
			irr::core::line3df m_vLineRelNode;
			bool m_bDrawSquares;
			float m_fPrecision;
			irr::core::vector3df* m_vLastPosAbs;
		};
	//====================================
	}
}

#endif // COLLGEOMETRY_HPP_INCLUDED
