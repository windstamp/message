/*
 *
 */

#ifndef __CMD_H__
#define __CMD_H__

enum ECmdId
{
	ECI_Null	= -1,

	ECI_Start	= 0,
	ECI_Login	= ECI_Start + 1,

	ECI_End,
};

#endif	/* __CMD_H__ */
