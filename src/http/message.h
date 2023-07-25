#pragma once

#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

#include "../single/encipher.h"

// 表示http信息处理状态
enum MSGSTATUS{
    HANDLE_INIT,
    HANDLE_HEAD,
    HANDLE_BODY,
    HANDLE_COMPLATE,
    HANDLE_ERROE,
};

// 表示消息体类型
enum MSGBODYTYPE{
    FILE_TYPE,
    TEXT_TYPE,
    EMPTY_TYPE,
};

// 表示文件处理过程
enum FILEMSGESTATUS{
    FILE_BEGIN,
    FILE_HEAD,
    FILE_CONTENT,
    FILE_COMPLATE
};

class Message{
public:
    Message() : HeadStatus(HANDLE_INIT), BodySatus(EMPTY_TYPE){}

public:
    int fd_;

    MSGSTATUS HeadStatus;
    MSGBODYTYPE BodySatus;

    std::unordered_map<std::string, std::string> MsgHeader;
};

class Request : public Message{
public:
    Request() : Message(){}

    void setRequestLine(const std::string &Line){
        std::istringstream lineStream(Line);
        lineStream >> Method;
        lineStream >> Resource;
        lineStream >> Version;

        if(Resource == "/")
            Resource = "/index";
    }


public:
    // buffer
    std::string RecvMsg;

    std::string Method;
    std::string Resource;
    std::string Version;

    std::string recvFileName;

    unsigned int BodyLen;

    FILEMSGESTATUS FileStatus;
    static const std::unordered_set<std::string> DEFAULT_HTML;
};

class Response : public Message{
public:
    Response() : Message(), HasSentLen(0){}

public:
    // 状态行
    std::string Version = "HTTP/1.1";
    std::string StatusCode;
    std::string StatusDes;

    std::string bodyFileName;

    std::string beforeBodyMsg;
    int beforeBodyMsgLen;

    std::string MsgBody;
    unsigned long long MsgBodyLen;

    int FileMsgFd;

    unsigned long long HasSentLen;

    static const std::unordered_map<int, std::string> CODE_STATUS, CODE_PATH;
};