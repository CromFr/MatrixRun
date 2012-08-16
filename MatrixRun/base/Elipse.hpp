#ifndef ELIPSE_HPP_INCLUDED
#define ELIPSE_HPP_INCLUDED

#include <irrlicht.h>
using namespace irr;

#include <list>
using namespace std;

/*
Elipse :
x²/a²+y²/b²=1
y² = (1 - x²/a²)*b²
y = +/- b*sqrt(1 - x²/a²)
*/

namespace base
{

	class Elipse
	{
	public:
		Elipse()
		{
			m_fA = 0;
			m_fB = 0;
			m_fAngle = 0;
			m_nPrecision = 15;

			m_bCalculated = true;
		}

		Elipse(const core::vector2df& vCenter, float fA, float fB, float fAngle, int nPrecision=15) :
			m_vCenter(vCenter)
		{
			m_fA = fA;
			m_fB = fB;
			m_fAngle = fAngle;
			m_nPrecision = nPrecision;

			m_bCalculated = false;
		}

		//Getter & Setter
		void SetCenter(const core::vector2df& v){m_vCenter=v; m_bCalculated=false;}
		void SetA(float f){m_fA=f; m_bCalculated=false;}
		void SetB(float f){m_fB=f; m_bCalculated=false;}
		void SetAngle(float f){m_fAngle=f; m_bCalculated=false;}
		void SetCalcPrecision(int i){m_nPrecision=i; m_bCalculated=false;}

		const core::vector2df& GetCenter()const {return m_vCenter;}
		float GetA()const {return m_fA;}
		float GetB()const {return m_fB;}
		float GetAngle()const {return m_fAngle;}
		int GetCalcPrecision()const {return m_nPrecision;}



		list<core::vector2df>* GetCalculatedPoints()
		{
			if(!m_bCalculated)
			{
				//Calculate points


				//Add special points
				m_listElipsePoints.push_back(core::vector2df(0,m_fB));
				m_listElipsePoints.push_back(core::vector2df(0,-m_fB));
				m_listElipsePoints.push_back(core::vector2df(m_fA,0));
				m_listElipsePoints.push_back(core::vector2df(-m_fA,0));

				//
				list<core::vector2df>::iterator it;
				if(m_fA>=m_fB)
				{
					//Calculate y with x fixed
					float a2 = m_fA*m_fA;
					for(int i=1 ; i<(m_nPrecision-1) ; i++)
					{
						float x = m_fA*i/(m_nPrecision-1);
						float y = m_fB*sqrt((1-x*x/a2));

						m_listElipsePoints.push_back(core::vector2df(x,y));
						m_listElipsePoints.push_back(core::vector2df(x,-y));
						m_listElipsePoints.push_back(core::vector2df(-x,-y));
						m_listElipsePoints.push_back(core::vector2df(-x,y));
					}
				}
				else
				{
					//Calculate x with y fixed
					float b2 = m_fB*m_fB;
					for(int i=1 ; i<(m_nPrecision-1) ; i++)
					{
						float y = m_fB*i/(m_nPrecision-1);
						float x = m_fA*sqrt((1-y*y/b2));

						m_listElipsePoints.push_back(core::vector2df(x,y));
						m_listElipsePoints.push_back(core::vector2df(x,-y));
						m_listElipsePoints.push_back(core::vector2df(-x,-y));
						m_listElipsePoints.push_back(core::vector2df(-x,y));
					}
				}

				//Rotate then translate the vectors
				float fCos = cosf(m_fAngle*3.14/180);
				float fSin = sinf(m_fAngle*3.14/180);
				int nCount=0;
				for(it=m_listElipsePoints.begin() ; it!=m_listElipsePoints.end() ; it++, nCount++)
				{
					float fNewX = fCos*it->X - fSin*it->Y		+m_vCenter.X;
					float fNewY = fSin*it->X + fCos*it->Y		+m_vCenter.Y;
					it->X = fNewX;
					it->Y = fNewY;
				}
				cout<<"Elipse created : "<<nCount<<" points :"<<endl;

				m_listElipsePoints.sort();
				for(it=m_listElipsePoints.begin() ; it!=m_listElipsePoints.end() ; it++)
					cout<<"Point at "<<it->X<<"\t"<<it->Y<<endl;
				m_bCalculated=true;
			}
			return &m_listElipsePoints;
		}


		bool GetIsInto(const core::vector2df& pos)const
		{
			return GetIsInto(pos.X, pos.Y);
		}

		bool GetIsInto(const float& fposX, const float& fposY)const
		{
			//Unrotate
			float fCos = cosf(-m_fAngle*3.14/180);
			float fSin = sinf(-m_fAngle*3.14/180);

			//Calculate point in elipse origin
			float fX = fCos*(fposX-m_vCenter.X) - fSin*(fposY-m_vCenter.Y);
			float fY = fSin*(fposX-m_vCenter.X) + fCos*(fposY-m_vCenter.Y);

			//use elipse formula
			return ( ((fX*fX)/(m_fA*m_fA) + (fY*fY)/(m_fB*m_fB)) <= 1 );
		}
	private:

		core::vector2df m_vCenter;
		float m_fA;
		float m_fB;
		float m_fAngle;

		int m_nPrecision;

		bool m_bCalculated;
		list<core::vector2df> m_listElipsePoints;

	};

}

#endif // ELIPSE_HPP_INCLUDED
