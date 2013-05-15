/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*	Class: palringoGroup
 *	handles group information gathering
 */

#ifndef PALRINGOGROUP_H
#define PALRINGOGROUP_H

class palringoGroup
{
	public:
		palringoGroup();

		void name(void);		//gets the groups name based on id
		void id(void);			//
		void description(void);	//gets groups description based on id
		void members(void);		//gets the members currently in the group

	protected:
	private:
};

#endif // PALRINGOGROUP_H
