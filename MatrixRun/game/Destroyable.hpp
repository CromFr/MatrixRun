#ifndef DESTROYABLE_HPP_INCLUDED
#define DESTROYABLE_HPP_INCLUDED

#include "Object.hpp"
#include "../irr/IEmptySceneNode.h"

namespace game
{
	/**
	@brief Abstract class for objects that can be destroyed
	**/
	class Destroyable : public Object
	{
	public:
		Destroyable(int nMaxHP)
		{
			m_nMaxHP = nMaxHP;
			m_nCurHP = m_nMaxHP;
		}


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


		int GetHP()const {return m_nCurHP;}

		bool GetIsDead()const {return !m_nCurHP;}

	protected:
		/**
		@brief Callback function executed when the object has been damaged
		@note this function is called before removing hit points
		@warning You should not call this function manually
		**/
		virtual void OnDamaged(int nAmount)=0;

		/**
		@brief Callback function executed when the object's hit points reaches 0 (death)
		@note this function is called after removing hit points, this means the current hit points =0
		@warning You should not call this function manually
		**/
		virtual void OnDeath()=0;

		int m_nMaxHP;
		int m_nCurHP;




	};
}




#endif // DESTROYABLE_HPP_INCLUDED
