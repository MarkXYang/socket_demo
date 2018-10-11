#ifndef _COMMON_H_
#define _COMMON_H_

#define PORT_ID				7856

#define BUF_SIZE			1024

enum err_type {NO_ERROR, WRONG_CHKSUM, WRONG_GROUP_ID, WRONG_MESSAGE_ID};

enum group_id {ID_CARMCU=1, ID_V2X, ID_NAD};

#endif  /* #ifndef _COMMON_H_*/
