//
// Created by zhu on 2018/6/7.
//
#include <jni.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>

// Max log message length
#define MAX_LOG_MESSAGE_LENGTH 256

// Max data buffer size
#define MAX_BUFFER_SIZE 80


/**
 * Logs the given message to the application.
 *
 * @param env JNIEnv interface.
 * @param obj object instance.
 * @param format message format and arguments.
 */
static void LogMessage(
        JNIEnv *env,
        jobject obj,
        const char *format,
        ...) {
    static jmethodID methodID = NULL;
    if (NULL == methodID) {
        jclass clazz = env->GetObjectClass(obj);
        methodID = env->GetMethodID(clazz,
                                    "logMessage",
                                    "(Ljava/lang/String;)V");
        env->DeleteLocalRef(clazz);
    }

    if (NULL != methodID) {
        // Format the log message
        char buffer[MAX_LOG_MESSAGE_LENGTH];

        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, MAX_LOG_MESSAGE_LENGTH, format, ap);
        va_end(ap);

        jstring message = env->NewStringUTF(buffer);

        if (NULL != message) {
            env->CallVoidMethod(obj, methodID, message);

            env->DeleteLocalRef(message);
        }
    }
}


static void ThrowException(JNIEnv *env,
                           const char *className,
                           const char *message) {
    jclass clazz = env->FindClass(
            className
    );

    if (NULL != clazz) {
        env->ThrowNew(clazz, message);
        env->DeleteLocalRef(clazz);
    }
}

static void ThrowErrnoException(
        JNIEnv *env,
        const char *className,
        int errnum
) {
    char buffer[MAX_LOG_MESSAGE_LENGTH];
    if (-1 == strerror_r(errnum, buffer, MAX_LOG_MESSAGE_LENGTH)) {
        strerror_r(errno,
                   buffer,
                   MAX_LOG_MESSAGE_LENGTH);
    }
    ThrowException(env, className, buffer);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_massky_shareptr_EchoServerActivity_nativeStartTcpServer(JNIEnv *env, jobject instance,
                                                                 jint port) {

    // TODO

}

static int NewTcpSocket(JNIEnv *env, jobject obj) {
    LogMessage(env, obj, "Constructing a new TCP socjet...");
    int tcpSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == tcpSocket) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    }
    return tcpSocket;
}

static void BindSocketToPort(JNIEnv *env,
                             jobject obj,
                             int sd,
                             unsigned short port) {
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = PF_INET;
    address.sin_port = htons(port);
    // Bind socket
    LogMessage(env, obj, "Binding to port %hu.", port);
    if (-1 == bind(sd, (struct sockaddr *) &address, sizeof(address))) {
        // Throw an exception with error number
        ThrowErrnoException(env, "java/io/IOException", errno);
    }

}

/**
 * Gets the port number socket is currently binded.
 *
 * @param env JNIEnv interface.
 * @param obj object instance.
 * @param sd socket descriptor.
 * @return port number.
 * @throws IOException
 */
static unsigned short GetSocketPort(
        JNIEnv *env,
        jobject obj,
        int sd) {
    unsigned short port = 0;

    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);

    // Get the socket address
    if (-1 == getsockname(sd, (struct sockaddr *) &address, &addressLength)) {
        // Throw an exception with error number
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        // Convert port to host byte order
        port = ntohs(address.sin_port);

        LogMessage(env, obj, "Binded to random port %hu.", port);
    }

    return port;
}


/**
 * Listens on given socket with the given backlog for
 * pending connections. When the backlog is full, the
 * new connections will be rejected.
 *
 * @param env JNIEnv interface.
 * @param obj object instance.
 * @param sd socket descriptor.
 * @param backlog backlog size.
 * @throws IOException
 */
static void ListenOnSocket(
        JNIEnv *env,
        jobject obj,
        int sd,
        int backlog) {
    // Listen on socket with the given backlog
    LogMessage(env, obj,
               "Listening on socket with a backlog of %d pending connections.",
               backlog);

    if (-1 == listen(sd, backlog)) {
        // Throw an exception with error number
        ThrowErrnoException(env, "java/io/IOException", errno);
    }
}

/**
 * Logs the IP address and the port number from the
 * given address.
 *
 * @param env JNIEnv interface.
 * @param obj object instance.
 * @param message message text.
 * @param address adress instance.
 * @throws IOException
 */
static void LogAddress(
        JNIEnv *env,
        jobject obj,
        const char *message,
        const struct sockaddr_in *address) {
    char ip[INET_ADDRSTRLEN];

    // Convert the IP address to string
    if (NULL == inet_ntop(PF_INET,
                          &(address->sin_addr),
                          ip,
                          INET_ADDRSTRLEN)) {
        // Throw an exception with error number
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        // Convert port to host byte order
        unsigned short port = ntohs(address->sin_port);

        // Log address
        LogMessage(env, obj, "%s %s:%hu.", message, ip, port);
    }
}

/**
 * Blocks and waits for incoming client connections on the
 * given socket.
 *
 * @param env JNIEnv interface.
 * @param obj object instance.
 * @param sd socket descriptor.
 * @return client socket.
 * @throws IOException
 */
static int AcceptOnSocket(
        JNIEnv *env,
        jobject obj,
        int sd) {
    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);

    // Blocks and waits for an incoming client connection
    // and accepts it
    LogMessage(env, obj, "Waiting for a client connection...");

    int clientSocket = accept(sd,
                              (struct sockaddr *) &address,
                              &addressLength);

    // If client socket is not valid
    if (-1 == clientSocket) {
        // Throw an exception with error number
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        // Log address
        LogAddress(env, obj, "Client connection from ", &address);
    }

    return clientSocket;
}

/**
 * Block and receive data from the socket into the buffer.
 *
 * @param env JNIEnv interface.
 * @param obj object instance.
 * @param sd socket descriptor.
 * @param buffer data buffer.
 * @param bufferSize buffer size.
 * @return receive size.
 * @throws IOException
 */
static ssize_t ReceiveFromSocket(
        JNIEnv *env,
        jobject obj,
        int sd,
        char *buffer,
        size_t bufferSize) {
    // Block and receive data from the socket into the buffer
    LogMessage(env, obj, "Receiving from the socket...");
    ssize_t recvSize = recv(sd, buffer, bufferSize - 1, 0);

    // If receive is failed
    if (-1 == recvSize) {
        // Throw an exception with error number
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        // NULL terminate the buffer to make it a string
        buffer[recvSize] = NULL;

        // If data is received
        if (recvSize > 0) {
            LogMessage(env, obj, "Received %d bytes: %s", recvSize, buffer);
        } else {
            LogMessage(env, obj, "Client disconnected.");
        }
    }

    return recvSize;
}

/**
 * Send data buffer to the socket.
 *
 * @param env JNIEnv interface.
 * @param obj object instance.
 * @param sd socket descriptor.
 * @param buffer data buffer.
 * @param bufferSize buffer size.
 * @return sent size.
 * @throws IOException
 */
static ssize_t SendToSocket(
        JNIEnv *env,
        jobject obj,
        int sd,
        const char *buffer,
        size_t bufferSize) {
    // Send data buffer to the socket
    LogMessage(env, obj, "Sending to the socket...");
    ssize_t sentSize = send(sd, buffer, bufferSize, 0);

    // If send is failed
    if (-1 == sentSize) {
        // Throw an exception with error number
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        if (sentSize > 0) {
            LogMessage(env, obj, "Sent %d bytes: %s", sentSize, buffer);
        } else {
            LogMessage(env, obj, "Client disconnected.");
        }
    }

    return sentSize;
}

static void ConnectToAddress(JNIEnv *env, jobject instance,
                             int sd,
                             const char *ip,
                             unsigned short port) {
    LogMessage(env, instance, "Connecting to %s:%uh...", ip, port);

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = PF_INET;

    if (0 == inet_aton(ip, &(address.sin_addr))) {
        address.sin_port = htons(port);

        // Connect to address
        if (-1 == connect(sd, (const sockaddr *) &address, sizeof(address))) {
            // Throw an exception with error number
            ThrowErrnoException(env, "java/io/IOException", errno);
        } else {
            LogMessage(env, instance, "Connected.");
        }
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_com_massky_shareptr_EchoServerActivity_nativeStartUdpServer(JNIEnv *env, jobject instance,
                                                                 jint port) {
    //
    int serverSocket = NewTcpSocket(env, instance);

    if (NULL == env->ExceptionOccurred()) {
        BindSocketToPort(env, instance, serverSocket, (u_short) port);
        if (NULL != env->ExceptionOccurred()) {
            goto exit;
        }

        if (0 == port) {
            GetSocketPort(env, instance, serverSocket);
            if (NULL != env->ExceptionOccurred()) {
                goto exit;
            }
        }

        ListenOnSocket(env, instance, serverSocket, 4);
        if (NULL != env->ExceptionOccurred()) {
            goto exit;
        }

        int clientSocket = AcceptOnSocket(env, instance, serverSocket);

        if (NULL != env->ExceptionOccurred()) {
            goto exit;
        }

        char buffer[MAX_BUFFER_SIZE];

        ssize_t recvSize;
        ssize_t sentSize;

        while (1) {
            recvSize = ReceiveFromSocket(env, instance, clientSocket, buffer, (size_t) recvSize);

            if ((0 == recvSize) || (NULL != env->ExceptionOccurred())) {
                break;
            }

            sentSize = SendToSocket(env, instance, clientSocket,
                                    buffer, (size_t) recvSize);

            if ((0 == sentSize) || (NULL != env->ExceptionOccurred())) {
                break;
            }
        }

        close(clientSocket);
    }

    exit:
    if (serverSocket > 0) {
        close(serverSocket);
    }

}

extern "C"
JNIEXPORT void JNICALL
Java_com_massky_shareptr_EchoClientActivity_nativeStartTcpClient(JNIEnv *env, jobject instance,
                                                                 jstring ip_, jint port,
                                                                 jstring message_) {
    int clientSocket = NewTcpSocket(env, instance);

    if (NULL == env->ExceptionOccurred()) {
        const char *ip = env->GetStringUTFChars(ip_, 0);
        if (NULL == ip) {
            goto exit;
        }

        ConnectToAddress(env, instance, clientSocket, ip,
                         port);
        env->ReleaseStringUTFChars(ip_, ip);

        if (NULL != env->ExceptionOccurred()) {
            goto exit;
        }

        const char *message = env->GetStringUTFChars(message_, 0);

        if (NULL == message) {
            goto exit;
        }

        jsize messageSize = env->GetStringUTFLength(message_);

        SendToSocket(env, instance, clientSocket, message, messageSize);
        env->ReleaseStringUTFChars(message_, message);

        if (NULL != env->ExceptionOccurred()) {
            goto exit;
        }

        char buffer[MAX_BUFFER_SIZE];

        ReceiveFromSocket(env, instance, clientSocket, buffer,
                          MAX_BUFFER_SIZE);
    }

    // TODO

    exit:
    if (clientSocket > -1) {
        close(clientSocket);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_massky_shareptr_EchoClientActivity_nativeStartUdpClient(JNIEnv *env, jobject instance,
                                                                 jstring ip_, jint port,
                                                                 jstring message_) {
    const char *ip = env->GetStringUTFChars(ip_, 0);
    const char *message = env->GetStringUTFChars(message_, 0);

    // TODO

    env->ReleaseStringUTFChars(ip_, ip);
    env->ReleaseStringUTFChars(message_, message);
}