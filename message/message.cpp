#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>

#include "msg_login.pb.h"

#include "cmd.h"

#include "message.h"


//void Encode(const char* _msg, const int32_t _size, const int32_t _cmd_id)
//{
//	int32_t type = EMT_Protobuf;
//
//	memcpy(buf, &type, 4);
//	memcpy(buf + 4, &_cmd_id, 4);
//	memcpy(buf + 8, &_size, 4);
//	memcpy(buf + 12, _msg, _size);
//
//	return;
//}

void Encode(char* _dst_msg, const char* _src_msg, size_t _size, const int32_t _type, const int32_t _cmd_id)
{
	memcpy(_dst_msg, &_type, 4);
	memcpy(_dst_msg + 4, &_cmd_id, 4);
	memcpy(_dst_msg + 8, &_size, 4);
	memcpy(_dst_msg + 12, _src_msg, _size);

	return;
}

//void Decode(const char* _src_msg, char* _dst_msg, int32_t* _size)
//{
//	if (!_size)
//	{
//		std::cerr << "Null pointer _size." << std::endl;
//		return;
//	}
//
//	int32_t type = EMT_Null;
//	int32_t cmd_id = ECI_Null;
//
//	memcpy(&type, _src_msg, 4);
//	memcpy(&cmd_id, _src_msg + 4, 4);
//	memcpy(_size, _src_msg + 8, 4);
//	memcpy(_dst_msg, _src_msg + 12, *_size);
//}

void Decode(char* _dst_msg, const char* _src_msg, size_t& _dst_size)
{
	int32_t type = EMT_Null;
	int32_t cmd_id = ECI_Null;

	memcpy(&type, _src_msg, 4);
	memcpy(&cmd_id, _src_msg + 4, 4);
	memcpy(&_dst_size, _src_msg + 8, 4);
	memcpy(_dst_msg, _src_msg + 12, _dst_size);
}

void EncodePotobuf(char* _dst_msg, ::google::protobuf::Message& msg, const int32_t _cmd_id)
{
	int32_t type = EMT_Protobuf;

	size_t size = msg.ByteSize();

	std::string str;
	msg.SerializeToString(&str);

	memcpy(buf, &type, 4);
	memcpy(_dst_msg + 4, &_cmd_id, 4);
	memcpy(_dst_msg + 8, &size, 4);
	memcpy(_dst_msg + 12, str.c_str(), size);
}

::google::protobuf::Message* DecodeProtobuf(const char* _src_msg)
{
	if (!_src_msg)
		return NULL;

	int32_t type = EMT_Null;
	int32_t cmd_id = ECI_Null;

	char dst_buf[4096];
	size_t dst_size = 0;

	memcpy(&type, _src_msg, 4);
	memcpy(&cmd_id, _src_msg + 4, 4);
	memcpy(&dst_size, _src_msg + 8, 4);
	memcpy(dst_buf, _src_msg + 12, dst_size);

	if (type != EMT_Protobuf)
		return NULL;

	::google::protobuf::Message* msg = NULL;

	switch(cmd_id)
	{
	case ECI_Login:
		msg = new network::MC_Login();

		if (msg)
			msg->ParseFromArray(dst_buf, dst_size);
		break;

	default:
		break;
	}

	return msg;
}

void DecodeProtobuf(::google::protobuf::Message* _msg, const char* _src_msg)
{
	if (!_msg)
		return;

	if (!_src_msg)
		return;

	int32_t type = EMT_Null;
	int32_t cmd_id = ECI_Null;

	char dst_buf[4096];
	size_t dst_size = 0;

	memcpy(&type, _src_msg, 4);
	memcpy(&cmd_id, _src_msg + 4, 4);
	memcpy(&dst_size, _src_msg + 8, 4);
	memcpy(dst_buf, _src_msg + 12, dst_size);

	if (type != EMT_Protobuf)
		return;

	switch(cmd_id)
	{
	case ECI_Login:
		_msg->ParseFromArray(dst_buf, dst_size);
		break;

	default:
		break;
	}

	return;
}

int main(int argc, char* argv[])
{
	//{
	//	char msg[] = {'a', 'b', 'c', 'd', '\0'};

	//	int32_t size = sizeof(msg);
	//	assert(size == 5);

	//	Encode(msg, size, ECI_Login);

	//	printf("buf = %s\n", buf);
	//}

	//{
	//	char msg[4096];

	//	size_t size = 0;

	//	Decode(buf, msg, size);

	//	printf("msg = %s, size = %d\n", msg, size);
	//}

	{
		std::cout << "0: " << std::endl;

		{
			char msg[] = {'a', 'b', 'c', 'd', '\0'};

			int32_t size = sizeof(msg);
			assert(size == 5);

			Encode(buf, msg, size, EMT_Null, ECI_Null);

			printf("buf = %s\n", buf);
		}

		{
			char msg[4096];

			size_t size = 0;

			Decode(msg, buf, size);

			printf("msg = %s, size = %d\n", msg, size);
		}
	}

	//{
	//	network::MC_Login msg_login;

	//	msg_login.set_id(1111);
	//	msg_login.set_name("player one");

	//	std::ostringstream oss;

	//	if ( !msg_login.SerializeToOstream(&oss) )
	//	{
	//		std::cerr << "Failed to write msg_login to oss." << std::endl;
	//	}
	//	
	//	int32_t size = oss.str().size();
	//	std::cout << "size = " << size << std::endl;

	//	Encode(oss.str().c_str(), size, ECI_Login);

	//	printf("buf = %s\n", buf);
	//}

	//{
	//	network::MC_Login msg_login;

	//	char msg[4096];

	//	int32_t size = 0;

	//	Decode(buf, msg, &size);

	//	//printf("msg = %s, size = %d\n", msg, size);

	//	if ( !msg_login.ParseFromArray(msg, size) )
	//	{
	//		std::cerr << "Failed to parse msg_login" << std::endl;
	//	}

	//	std::cout << "id = " << msg_login.id() << std::endl;
	//	std::cout << "name = " << msg_login.name() << std::endl;

	//}

	{
		std::cout << std::endl << "1: " << std::endl;

		{
			network::MC_Login msg_login;

			msg_login.set_id(1111);
			msg_login.set_name("player one");

			std::string str;

			if ( !msg_login.SerializeToString(&str) )
			{
				std::cerr << "Failed to write msg_login to oss." << std::endl;
			}

			int32_t size = str.size();
			std::cout << "size = " << size << std::endl;

			Encode(buf, str.c_str(), size, EMT_Protobuf, ECI_Login);

			printf("buf = %s\n", buf);
		}

		{
			network::MC_Login msg_login;

			char msg[4096];

			size_t size = 0;

			Decode(msg, buf, size);

			if ( !msg_login.ParseFromArray(msg, size) )
			{
				std::cerr << "Failed to parse msg_login" << std::endl;
			}

			std::cout << "id = " << msg_login.id() << std::endl;
			std::cout << "name = " << msg_login.name() << std::endl;

		}
	}

	{
		std::cout << std::endl << "2: " << std::endl;

		{
			network::MC_Login msg_login;

			msg_login.set_id(1111);
			msg_login.set_name("player one");

			EncodePotobuf(buf, msg_login, ECI_Login);

			printf("buf = %s\n", buf);
		}

		{
			network::MC_Login msg_login;

			char msg[4096];

			size_t size = 0;

			Decode(msg, buf, size);

			if ( !msg_login.ParseFromArray(msg, size) )
			{
				std::cerr << "Failed to parse msg_login" << std::endl;
			}

			std::cout << "id = " << msg_login.id() << std::endl;
			std::cout << "name = " << msg_login.name() << std::endl;

		}
	}

	{
		std::cout << std::endl << "3: " << std::endl;

		{
			network::MC_Login msg_login;

			msg_login.set_id(1111);
			msg_login.set_name("player one");

			EncodePotobuf(buf, msg_login, ECI_Login);

			printf("buf = %s\n", buf);
		}

		{
			::google::protobuf::Message* msg = DecodeProtobuf(buf);

			network::MC_Login* msg_login = dynamic_cast<network::MC_Login*>(msg);

			if (msg_login)
			{
				std::cout << "id = " << msg_login->id() << std::endl;
				std::cout << "name = " << msg_login->name() << std::endl;
			}

			if (msg)
			{
				delete msg;
				msg = NULL;
			}
		}
	}

	{
		std::cout << std::endl << "4: " << std::endl;

		{
			network::MC_Login msg_login;

			msg_login.set_id(1111);
			msg_login.set_name("player one");

			EncodePotobuf(buf, msg_login, ECI_Login);

			printf("buf = %s\n", buf);
		}

		{
			network::MC_Login msg_login;

			DecodeProtobuf(&msg_login, buf);

			std::cout << "id = " << msg_login.id() << std::endl;
			std::cout << "name = " << msg_login.name() << std::endl;
		}
	}

	getchar();
	return 0;
}