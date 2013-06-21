#ifndef ELIPSE_HPP_INCLUDED
#define ELIPSE_HPP_INCLUDED

#include <irrlicht.h>

#include <cmath>
#include <list>

/*
Ellipse :
x²/a²+y²/b²=1
y² = (1 - x²/a²)*b²
y = +/- b*sqrt(1 - x²/a²)
*/

namespace base
{
	/**
	@brief A 2d ellipse model
	**/
	class Ellipse
	{
	public:
		/**
		@brief Create a default ellipse with all parameters set to 0
		**/
		Ellipse();

		/**
		@brief Create an ellipse with given parameters
		@param vCenter The origin of the ellipse in 2 dimensions
		@param fA the half size of the ellipse on the X axis
		@param fB the half size of the ellipse on the Y axis
		@param nPrecision the number of points to calculate on a quart of ellipse (total points = nPrecision*4-4)
		@note Does not calculate points
		**/
		Ellipse(const irr::core::vector2df& vCenter, float fA, float fB, float fAngle, int nPrecision=15);

		/**
		@brief Copy constructor
		@warning You need to recalculate points of the new ellipse
		**/
		Ellipse(const Ellipse& b);

		/**
		@brief Operator =
		@warning You need to recalculate points of the ellipse on the left side
		**/
		Ellipse& operator=(const Ellipse& b);

		//Getter & Setter
		void SetCenter(const irr::core::vector2df& v){m_vCenter=v; m_bCalculated=false;}
		void SetA(float f){m_fA=f; m_bCalculated=false;}
		void SetB(float f){m_fB=f; m_bCalculated=false;}
		void SetAngle(float f){m_fAngle=f; m_fCos=cosf(m_fAngle*3.14/180); m_fSin=sinf(m_fAngle*3.14/180); m_bCalculated=false;}
		///@see constructor
		void SetCalcPrecision(int i){m_nPrecision=i; m_bCalculated=false;}

		const irr::core::vector2df& GetCenter()const {return m_vCenter;}
		float GetA()const {return m_fA;}
		float GetB()const {return m_fB;}
		float GetAngle()const {return m_fAngle;}
		///@see constructor
		int GetCalcPrecision()const {return m_nPrecision;}


		/**
		@brief Calculates points of the ellipse
		@return The list that contain all points in 2d coordinates and sorted by x
		**/
		std::list<irr::core::vector2df>* GetCalculatedPoints();

		/**
		@brief Gets if the point is inside or outside the ellipse
		@param pos The coordinates of the point to check
		@return true if the point is inside, false if outside
		**/
		bool GetIsInto(const irr::core::vector2df& pos)const {return GetIsInto(pos.X, pos.Y);}


		/**
		@brief Gets if the point is inside or outside the ellipse
		@param x The X coordinates of the point to check
		@param x The Y coordinates of the point to check
		@return true if the point is inside, false if outside
		**/
		bool GetIsInto(const float& fposX, const float& fposY)const;

		/**
		@brief
		**/
		irr::core::vector2df GetPointReportedOnEllipse(const irr::core::vector2df& pos);

	//=====================
	private:

		irr::core::vector2df m_vCenter;
		float m_fA;
		float m_fB;
		float m_fAngle;
		float m_fCos;
		float m_fSin;

		int m_nPrecision;

		bool m_bCalculated;
		std::list<irr::core::vector2df> m_listEllipsePoints;

	};

}

#endif // ELIPSE_HPP_INCLUDED
