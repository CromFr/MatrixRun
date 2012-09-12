#include "Ellipse.hpp"

using namespace irr;
using namespace std;

namespace base
{
	/*====================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	====================================================================================================================*/
	Ellipse::Ellipse()
	{
		m_fA = 0;
		m_fB = 0;
		m_fAngle = 0;
		m_fCos = 0;
		m_fSin = 1;
		m_nPrecision = 15;

		m_bCalculated = true;
	}


	/*====================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	====================================================================================================================*/
	Ellipse::Ellipse(const core::vector2df& vCenter, float fA, float fB, float fAngle, int nPrecision) :
		m_vCenter(vCenter)
	{
		m_fA = fA;
		m_fB = fB;
		m_fAngle = fAngle;
		m_fCos=cosf(m_fAngle*3.14/180);
		m_fSin=sinf(m_fAngle*3.14/180);
		m_nPrecision = nPrecision;

		m_bCalculated = false;
	}


	/*====================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	====================================================================================================================*/
	list<core::vector2df>* Ellipse::GetCalculatedPoints()
	{
		if(!m_bCalculated)
		{
			//Calculate points

			//Add special points
			m_listEllipsePoints.push_back(core::vector2df(0,m_fB));
			m_listEllipsePoints.push_back(core::vector2df(0,-m_fB));
			m_listEllipsePoints.push_back(core::vector2df(m_fA,0));
			m_listEllipsePoints.push_back(core::vector2df(-m_fA,0));

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

					m_listEllipsePoints.push_back(core::vector2df(x,y));
					m_listEllipsePoints.push_back(core::vector2df(x,-y));
					m_listEllipsePoints.push_back(core::vector2df(-x,-y));
					m_listEllipsePoints.push_back(core::vector2df(-x,y));
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

					m_listEllipsePoints.push_back(core::vector2df(x,y));
					m_listEllipsePoints.push_back(core::vector2df(x,-y));
					m_listEllipsePoints.push_back(core::vector2df(-x,-y));
					m_listEllipsePoints.push_back(core::vector2df(-x,y));
				}
			}

			//Rotate then translate the vectors
			int nCount=0;
			for(it=m_listEllipsePoints.begin() ; it!=m_listEllipsePoints.end() ; it++, nCount++)
			{
				float fNewX = m_fCos*it->X - m_fSin*it->Y		+m_vCenter.X;
				float fNewY = m_fSin*it->X + m_fCos*it->Y		+m_vCenter.Y;
				it->X = fNewX;
				it->Y = fNewY;
			}

			m_listEllipsePoints.sort();
			m_bCalculated=true;
		}
		return &m_listEllipsePoints;
	}


	/*====================================================================================================================
	\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	====================================================================================================================*/
	bool Ellipse::GetIsInto(const float& fposX, const float& fposY)const
	{
		//Unrotate
		float fCos = m_fCos;//cosf(-m_fAngle*3.14/180);
		float fSin = -m_fSin;//sinf(-m_fAngle*3.14/180);

		//Calculate point in ellipse origin
		float fX = fCos*(fposX-m_vCenter.X) - fSin*(fposY-m_vCenter.Y);
		float fY = fSin*(fposX-m_vCenter.X) + fCos*(fposY-m_vCenter.Y);

		//use ellipse formula
		return ( ((fX*fX)/(m_fA*m_fA) + (fY*fY)/(m_fB*m_fB)) <= 1 );
	}

}
