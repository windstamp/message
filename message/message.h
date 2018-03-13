/*
 *
 */

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#define MESSAGE_MAX_LENGTH	4096

char buf[MESSAGE_MAX_LENGTH];

// 消息定义
// type(四个字节) + 协议ID(四个字节) + 协议内容长度(四个字节) + 协议内容

enum EMessageType
{
	EMT_Null		= -1,

	EMT_Start		= 0,
	EMT_Protobuf	= EMT_Start + 1,

	EMT_End,
};

//void Encode(const char* _msg, const int32_t _size, const int32_t _cmd_id);

void Encode(char* _dst_msg, const char* _src_msg, size_t _size, const int32_t _type, const int32_t _cmd_id);

//void Decode(const char* _src_msg, char* _dst_msg, int32_t* _size);

void Decode(char* _dst_msg, const char* _src_msg, size_t& _dst_size);

void EncodePotobuf(char* _dst_msg, ::google::protobuf::Message& msg, const int32_t _cmd_id);

::google::protobuf::Message* DecodeProtobuf(const char* _src_msg);

void DecodeProtobuf(::google::protobuf::Message* _msg, const char* _src_msg);

#endif	/* __MESSAGE_H__ */
