/************************************************************************************************
* Author         : Labhesh Patil
* Date           : December 18, 2025
* File           : password.h
* Title          : Password management header
* Description    : Function declarations for password comparison and change operations.
*                 Defines interface for old_password() and new_password() entry stages.
*****************************************************************************************************/
#ifndef PASSWORD_H
#define	PASSWORD_H

// Password management function declaration
int compare_pass(const char *p1, const char *p2);
int change_password(void);
unsigned int old_password(void);
unsigned int new_password(void);

#endif	/* PASSWORD_H */