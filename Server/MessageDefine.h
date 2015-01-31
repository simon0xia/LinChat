#ifndef _MESSAGEDEFINE_H
#define  _MESSAGEDEFINE_H

enum MessageType
{
	Heartbeat,
	Login,
	Logout,
	Message = 50,


	Ack_Heartbeat=500,
	Ack_Login,
	Ack_logout,
	Ack_message=550,


	MaxMessage
};

#endif //#ifndef _MESSAGEDEFINE_H