/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*	Class: palringoContact
 *	handles person information gathering
 */

#ifndef PALRINGOCONTACT_H
#define PALRINGOCONTACT_H

class palringoContact
{
	public:
		palringoContact();

		void getName(void); //get persons name based on id
		void getID(void);	//

		void add(void);		//add someone to your contacts list by using their id
		void accept(void);	//accept someones contact request
		void decline(void);	//decline someones contact request
		void block(void);	//block a user from messaging you

	protected:
	private:
};

#endif // PALRINGOCONTACT_H
