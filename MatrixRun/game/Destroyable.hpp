#ifndef DESTROYABLE_HPP_INCLUDED
#define DESTROYABLE_HPP_INCLUDED

#include "Object.hpp"
#include "../irr/IEmptySceneNode.h"

namespace game
{
	class Destroyable : public Object
	{
	public:
		Destroyable(int nMaxHP)
		{
			m_nMaxHP = nMaxHP;
			m_nCurHP = m_nMaxHP;
		}


		int GetHP(){return m_nCurHP;}

		void ApplyDamage(int nAmount)
		{
			OnDamaged(nAmount);

			if(m_nCurHP>0)
			{
				m_nCurHP -= nAmount;
				if(m_nCurHP<0)
				{
					m_nCurHP=0;
					OnDeath();
				}
			}


		}

		void Heal(int nAmount)
		{
			m_nCurHP += nAmount;
			if(m_nCurHP>m_nMaxHP)
				m_nCurHP=m_nMaxHP;
		}

		bool GetIsDead()
		{
			return !m_nCurHP;
		}

	private:
		virtual void OnDamaged(int nAmount)=0;
		virtual void OnDeath()=0;

		int m_nMaxHP;
		int m_nCurHP;




	};
}




#endif // DESTROYABLE_HPP_INCLUDED
