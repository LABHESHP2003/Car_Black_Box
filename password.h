/* 
 * File:   password.h
 * Author: labhe
 *
 * Created on February 23, 2026, 12:37 PM
 */

#ifndef PASSWORD_H
#define	PASSWORD_H

int compare_pass(const char *p1, const char *p2);
int change_password(void);
unsigned int old_password();
unsigned int new_password();

#endif	/* PASSWORD_H */

