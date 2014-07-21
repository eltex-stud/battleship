/* vim: set noexpandtab tabstop=4 shiftwidth=4 smartindent: */
/*
 * ip - loopback
 * port -  1234
 *
 * net_types.h
 *
 * net_prototype msg
 *		type NICK
 *			1 byte - type message
 *			2 byte - nickname
 *		type PLACEMENT
 *			1 byte - type message
 *			2 byte - placement
 *		type START
 *			1 byte - type message
 *			2 byte - turn
 *		type SHOT
 *			1 byte - type message
 *			2 byte - value x
 *			3 byte - value y
 *			4 byte - result of shot
 *		type ERROR
 *			1 byte - type message
 *			2 byte - error code
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>

enum types_msg{
	NICK,
	PLACEMENT,
	START,
	SHOT,
	ERROR
};
