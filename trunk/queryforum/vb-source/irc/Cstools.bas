Attribute VB_Name = "Module1"
Global MyName$
Global TheirName$
Global Code$
Declare Function sndPlaySound Lib "winmm" Alias "sndPlaySoundA" (ByVal lpszSoundName As String, ByVal uFlags As Long) As Long
'
' Catalyst SocketTools 1.0
' Copyright (c) 1995-1996, Catalyst Development Corp. All rights reserved.
'
' This file contains the constants and function declarations used
' with the SocketTools TCP/IP development toolkit for Visual Basic 3.0
'

'
' General constants used with most of the controls
'
Global Const INVALID_HANDLE = -1
Global Const CONTROL_ERRIGNORE = 0
Global Const CONTROL_ERRDISPLAY = 1

'
' Windows Sockets error codes. These error codes are returned by
' the DNS (Domain Name Service), Ping and SocketWrench controls.
'
Global Const WSABASEERR = 24000
Global Const WSAEINTR = 24004
Global Const WSAEBADF = 24009
Global Const WSAEACCES = 24013
Global Const WSAEFAULT = 24014
Global Const WSAEINVAL = 24022
Global Const WSAEMFILE = 24024
Global Const WSAEWOULDBLOCK = 24035
Global Const WSAEINPROGRESS = 24036
Global Const WSAEALREADY = 24037
Global Const WSAENOTSOCK = 24038
Global Const WSAEDESTADDRREQ = 24039
Global Const WSAEMSGSIZE = 24040
Global Const WSAEPROTOTYPE = 24041
Global Const WSAENOPROTOOPT = 24042
Global Const WSAEPROTONOSUPPORT = 24043
Global Const WSAESOCKTNOSUPPORT = 24044
Global Const WSAEOPNOTSUPP = 24045
Global Const WSAEPFNOSUPPORT = 24046
Global Const WSAEAFNOSUPPORT = 24047
Global Const WSAEADDRINUSE = 24048
Global Const WSAEADDRNOTAVAIL = 24049
Global Const WSAENETDOWN = 24050
Global Const WSAENETUNREACH = 24051
Global Const WSAENETRESET = 24052
Global Const WSAECONNABORTED = 24053
Global Const WSAECONNRESET = 24054
Global Const WSAENOBUFS = 24055
Global Const WSAEISCONN = 24056
Global Const WSAENOTCONN = 24057
Global Const WSAESHUTDOWN = 24058
Global Const WSAETOOMANYREFS = 24059
Global Const WSAETIMEDOUT = 24060
Global Const WSAECONNREFUSED = 24061
Global Const WSAELOOP = 24062
Global Const WSAENAMETOOLONG = 24063
Global Const WSAEHOSTDOWN = 24064
Global Const WSAEHOSTUNREACH = 24065
Global Const WSAENOTEMPTY = 24066
Global Const WSAEPROCLIM = 24067
Global Const WSAEUSERS = 24068
Global Const WSAEDQUOT = 24069
Global Const WSAESTALE = 24070
Global Const WSAEREMOTE = 24071
Global Const WSASYSNOTREADY = 24091
Global Const WSAVERNOTSUPPORTED = 24092
Global Const WSANOTINITIALISED = 24093
Global Const WSAHOST_NOT_FOUND = 25001
Global Const WSATRY_AGAIN = 25002
Global Const WSANO_RECOVERY = 25003
Global Const WSANO_DATA = 25004
Global Const WSANO_ADDRESS = 25004

'
' Constants used with the Domain Name Service (DNS) cotrol
'
' DNS client actions
'
Global Const DNS_ACTION_NONE = 0
Global Const DNS_ACTION_CANCEL = 1

'
' Constants used with the File Transfer Protocol (FTP) control.
'
' The error codes are returned by the LastError property and
' passed as an argument to the LastError event.
'
Global Const FTP_ERROR = -1
Global Const ERROR_FTP_NOHANDLES = 22001    ' no free handles
Global Const ERROR_FTP_ALLOCMEM = 22002     ' no global memory
Global Const ERROR_FTP_NOTOWNER = 22003     ' not session owner
Global Const ERROR_FTP_NETSTART = 22004     ' cannot initialize network
Global Const ERROR_FTP_HOSTNAME = 22005     ' invalid hostname
Global Const ERROR_FTP_SOCKET = 22006       ' cannot create socket
Global Const ERROR_FTP_ACCEPT = 22007       ' cannot accept connection
Global Const ERROR_FTP_CONNECT = 22008      ' cannot connect to server
Global Const ERROR_FTP_PEERNAME = 22009     ' cannot get peer information
Global Const ERROR_FTP_BIND = 22010         ' cannot bind socket
Global Const ERROR_FTP_LISTEN = 22011       ' cannot listen for connection
Global Const ERROR_FTP_PARAM = 22012        ' invalid parameter
Global Const ERROR_FTP_HANDLE = 22013       ' invalid FTP handle
Global Const ERROR_FTP_SELECT = 22014       ' socket select error
Global Const ERROR_FTP_READ = 22015         ' cannot read from socket
Global Const ERROR_FTP_WRITE = 22016        ' cannot write to socket
Global Const ERROR_FTP_EOF = 22017          ' end of file on data connection
Global Const ERROR_FTP_BUSY = 22018         ' connection in use
Global Const ERROR_FTP_NOTBUSY = 22019      ' connection is not in use
Global Const ERROR_FTP_TIMEOUT = 22020      ' timeout waiting for socket
Global Const ERROR_FTP_SYNC = 22021         ' internal synchronization error
Global Const ERROR_FTP_CLOSED = 22022       ' server closed connection
Global Const ERROR_FTP_NOTREADY = 22023     ' server is not ready, try again
Global Const ERROR_FTP_NOTAVAIL = 22024     ' remote server is not available
Global Const ERROR_FTP_SERVICE = 22025      ' unknown problem with remote server
Global Const ERROR_FTP_DATAPORT = 22026     ' unable to set data port
Global Const ERROR_FTP_DATAOPEN = 22027     ' data port already open
Global Const ERROR_FTP_DATACLOSED = 22028   ' data port is closed
Global Const ERROR_FTP_ABORTED = 22029      ' file transfer aborted
Global Const ERROR_FTP_READONLY = 22030     ' data connection is read-only
Global Const ERROR_FTP_WRITEONLY = 22031    ' data connection is write-only
Global Const ERROR_FTP_FILETYPE = 22032     ' unable to set file type
Global Const ERROR_FTP_PASSIVE = 22033      ' data connection is passive
Global Const ERROR_FTP_ADDRESS = 22034      ' cannot determine passive address
Global Const ERROR_FTP_CREATEFILE = 22035   ' cannot create local file
Global Const ERROR_FTP_OPENFILE = 22036     ' cannot open local file
Global Const ERROR_FTP_READFILE = 22037     ' cannot read local file
Global Const ERROR_FTP_WRITEFILE = 22038    ' cannot write local file
Global Const ERROR_FTP_LOGIN = 22039        ' cannot login to remote server
Global Const ERROR_FTP_OPENFAIL = 22040     ' cannot open data connection
Global Const ERROR_FTP_FILEBUSY = 22041     ' file unavailable
Global Const ERROR_FTP_NOSPACE = 22042      ' insufficient storage space
Global Const ERROR_FTP_COMMAND = 22043      ' command not recognized
Global Const ERROR_FTP_ARGUMENT = 22044     ' invalid command argument
Global Const ERROR_FTP_CMDIMP = 22045       ' command not implemented
Global Const ERROR_FTP_CMDSEQ = 22046       ' invalid command sequence
Global Const ERROR_FTP_NOLOGIN = 22047      ' not logged in
Global Const ERROR_FTP_NOACCOUNT = 22048    ' account required
Global Const ERROR_FTP_ACCESS = 22049       ' cannot access file on server
Global Const ERROR_FTP_FILEALLOC = 22050    ' file exceeded storage allocation
Global Const ERROR_FTP_FILENAME = 22051     ' invalid file name
Global Const ERROR_FTP_UNEXPECTED = 22052   ' unexpected server error

'
' Result codes returned by the FTP server. The ResultCode property
' specifies the code from the last operation.
'
Global Const FTP_REPLY_RESTMARK = 110   ' restart marker reply
Global Const FTP_REPLY_NOTREADY = 120   ' service available in n minutes
Global Const FTP_REPLY_DATAOPEN = 125   ' data connection open, transfer started
Global Const FTP_REPLY_FILEOK = 150     ' file status okay
Global Const FTP_REPLY_CMDOK = 200      ' command okay
Global Const FTP_REPLY_CMDIGN = 202     ' command ignored
Global Const FTP_REPLY_SYSSTAT = 211    ' system status
Global Const FTP_REPLY_DIRSTAT = 212    ' directory status
Global Const FTP_REPLY_FILESTAT = 213   ' file status
Global Const FTP_REPLY_HELPMSG = 214    ' human-readable help response
Global Const FTP_REPLY_READY = 220      ' service ready for new user
Global Const FTP_REPLY_CLOSED = 221     ' service closing connection
Global Const FTP_REPLY_DATAOPENED = 225 ' data connection open
Global Const FTP_REPLY_DATACLOSED = 226 ' closing data connection
Global Const FTP_REPLY_PASVMODE = 227   ' entering passive mode
Global Const FTP_REPLY_LOGIN = 230      ' user logged in
Global Const FTP_REPLY_DONE = 250       ' requested file action completed
Global Const FTP_REPLY_GETPASS = 331    ' username okay, need password
Global Const FTP_REPLY_GETACCT = 332    ' need account for login
Global Const FTP_REPLY_PENDING = 350    ' file action pending
Global Const FTP_REPLY_OPENFAIL = 425   ' cannot open data connection
Global Const FTP_REPLY_ABORTED = 426    ' conenct closed, transfer aborted
Global Const FTP_REPLY_NOTAVAIL = 450   ' file is not available
Global Const FTP_REPLY_LOCALERR = 451   ' local error
Global Const FTP_REPLY_NOSPACE = 452    ' no space on server system
Global Const FTP_REPLY_BADSYN = 500     ' syntax error
Global Const FTP_REPLY_BADARG = 501     ' invalid command arguments
Global Const FTP_REPLY_BADCMD = 502     ' command not implemented
Global Const FTP_REPLY_BADSEQ = 503     ' bad sequence of commands
Global Const FTP_REPLY_BADPARM = 504    ' bad command parameter
Global Const FTP_REPLY_NOLOGIN = 530    ' user not logged in
Global Const FTP_REPLY_ACCTREQ = 532    ' account required for storing files
Global Const FTP_REPLY_NOFILE = 550     ' file unavailable
Global Const FTP_REPLY_BADPAGE = 551    ' page type unknown
Global Const FTP_REPLY_EXQUOTA = 552    ' exceeded file storage quota
Global Const FTP_REPLY_BADFILE = 553    ' invalid file name

Global Const FTP_FILE_READ = 0          ' read from file on remote host
Global Const FTP_FILE_WRITE = 1         ' write to file on remote host
Global Const FTP_FILE_APPEND = 2        ' append to file on remote host
Global Const FTP_FILE_UNIQUE = 3        ' create unique file on remote host

'
' FTP file types
'
Global Const FILE_TYPE_ASCII = 0
Global Const FILE_TYPE_EBCDIC = 1
Global Const FILE_TYPE_IMAGE = 2

'
' FTP client actions
'
Global Const FTP_ACTION_NONE = 0
Global Const FTP_ACTION_CONNECT = 1
Global Const FTP_ACTION_LOGIN = 2
Global Const FTP_ACTION_LOGOUT = 3
Global Const FTP_ACTION_DISCONNECT = 4
Global Const FTP_ACTION_GETFILE = 5
Global Const FTP_ACTION_PUTFILE = 6
Global Const FTP_ACTION_FILELIST = 7
Global Const FTP_ACTION_OPEN = 8
Global Const FTP_ACTION_CREATE = 9
Global Const FTP_ACTION_APPEND = 10
Global Const FTP_ACTION_CLOSE = 11
Global Const FTP_ACTION_CANCEL = 12
Global Const FTP_ACTION_DELETE = 13
Global Const FTP_ACTION_RENAME = 14
Global Const FTP_ACTION_GETCWD = 15
Global Const FTP_ACTION_CHDIR = 16
Global Const FTP_ACTION_MKDIR = 17
Global Const FTP_ACTION_RMDIR = 18

'
' FTP client states
'
Global Const FTP_CLIENT_UNUSED = 0
Global Const FTP_CLIENT_IDLE = 1
Global Const FTP_CLIENT_CONNECTING = 2
Global Const FTP_CLIENT_LOGIN = 3
Global Const FTP_CLIENT_TRANSFER = 4

'
' Image viewer control constants
'
Global Const INVALID_IMAGE = 0

'
' Image types supported by the control
'
Global Const IMAGE_TYPE_NONE = 0
Global Const IMAGE_TYPE_BITMAP = 1
Global Const IMAGE_TYPE_GIF = 2
Global Const IMAGE_TYPE_PCX = 3
Global Const IMAGE_TYPE_JPEG = 4
Global Const IMAGE_TYPE_XBM = 5
Global Const IMAGE_TYPE_EXTERN = 6

'
' Network News Transfer Protocol (NNTP) control constants
'
' The error codes are returned by the LastError property and
' passed as an argument to the LastError event.
'
Global Const NNTP_ERROR = -1
Global Const ERROR_NNTP_HANDLE = 22301
Global Const ERROR_NNTP_PARAM = 22302
Global Const ERROR_NNTP_INIT = 22303
Global Const ERROR_NNTP_ALLOC = 22304
Global Const ERROR_NNTP_HOSTNAME = 22305
Global Const ERROR_NNTP_SOCKET = 22306
Global Const ERROR_NNTP_CONNECT = 22307
Global Const ERROR_NNTP_SELECT = 22308
Global Const ERROR_NNTP_TIMEOUT = 22309
Global Const ERROR_NNTP_SERVER = 22310
Global Const ERROR_NNTP_READ = 22311
Global Const ERROR_NNTP_WRITE = 22312
Global Const ERROR_NNTP_CLOSED = 22313
Global Const ERROR_NNTP_BUSY = 22314
Global Const ERROR_NNTP_ABORTED = 22315
Global Const ERROR_NNTP_NOSERVICE = 22316
Global Const ERROR_NNTP_BADGROUP = 22317
Global Const ERROR_NNTP_NOGROUP = 22318
Global Const ERROR_NNTP_EMPTYGROUP = 22319
Global Const ERROR_NNTP_NOARTICLE = 22320
Global Const ERROR_NNTP_LASTARTICLE = 22321
Global Const ERROR_NNTP_FIRSTARTICLE = 22322
Global Const ERROR_NNTP_BADARTICLE = 22323
Global Const ERROR_NNTP_NOTRANSFER = 22324
Global Const ERROR_NNTP_BADTRANSFER = 22325
Global Const ERROR_NNTP_REJECTED = 22326
Global Const ERROR_NNTP_NOPOSTING = 22327
Global Const ERROR_NNTP_BADPOST = 22328
Global Const ERROR_NNTP_COMMAND = 22329
Global Const ERROR_NNTP_RESTRICTED = 22330
Global Const ERROR_NNTP_FAULT = 22331
Global Const ERROR_NNTP_AUTHINFO = 22332
Global Const ERROR_NNTP_AUTHFAIL = 22333
Global Const ERROR_NNTP_AUTHTYPE = 22334
Global Const ERROR_NNTP_NOAUTH = 22335

'
' Result codes returned by the NNTP server. The ResultCode property
' specifies the code from the last operation.
'
Global Const NNTP_REPLY_READY = 200
Global Const NNTP_REPLY_SELECTED = 211
Global Const NNTP_REPLY_GROUPLIST = 215
Global Const NNTP_REPLY_ARTICLE = 220
Global Const NNTP_REPLY_HEADER = 221
Global Const NNTP_REPLY_BODY = 222
Global Const NNTP_REPLY_STATUS = 223
Global Const NNTP_REPLY_NEWARTICLES = 230
Global Const NNTP_REPLY_NEWGROUPS = 231
Global Const NNTP_REPLY_TRANSFERRED = 235
Global Const NNTP_REPLY_POSTED = 240
Global Const NNTP_REPLY_GROUPTITLE = 282
Global Const NNTP_REPLY_XFERARTICLE = 335
Global Const NNTP_REPLY_AUTHCONT = 350
Global Const NNTP_REPLY_AUTHPASS = 381
Global Const NNTP_REPLY_POSTARTICLE = 340
Global Const NNTP_REPLY_AUTHSIMPLE = 450
Global Const NNTP_REPLY_AUTHORIGIN = 480

'
' NNTP client actions
'
Global Const NNTP_ACTION_NONE = 0
Global Const NNTP_ACTION_CONNECT = 1
Global Const NNTP_ACTION_DISCONNECT = 2
Global Const NNTP_ACTION_LISTGROUPS = 3
Global Const NNTP_ACTION_NEWGROUPS = 4
Global Const NNTP_ACTION_LISTARTICLES = 5
Global Const NNTP_ACTION_GETHEADER = 6
Global Const NNTP_ACTION_GETARTICLE = 7
Global Const NNTP_ACTION_NEXTARTICLE = 8
Global Const NNTP_ACTION_PREVARTICLE = 9
Global Const NNTP_ACTION_NEWARTICLE = 10
Global Const NNTP_ACTION_POSTARTICLE = 11
Global Const NNTP_ACTION_AUTHENTICATE = 12

'
' NNTP client states
'
Global Const NNTP_CLIENT_UNUSED = 0
Global Const NNTP_CLIENT_IDLE = 1
Global Const NNTP_CLIENT_CONNECTING = 2
Global Const NNTP_CLIENT_GROUPLIST = 3
Global Const NNTP_CLIENT_ARTICLELIST = 4
Global Const NNTP_CLIENT_READING = 5
Global Const NNTP_CLIENT_POSTING = 6

'
' Post Office Protocol (POP) client control constants
'
' The error codes are returned by the LastError property and
' passed as an argument to the LastError event.
'
Global Const POP_ERROR = -1
Global Const ERROR_POP_HANDLE = 22301   ' invalid handle to pop client
Global Const ERROR_POP_PARAM = 22302    ' invalid paramter passed to function
Global Const ERROR_POP_INIT = 22303     ' unable to initialize socket library
Global Const ERROR_POP_ALLOC = 22304    ' unable to allocate memory
Global Const ERROR_POP_HOSTNAME = 22305 ' unable to resolve hostname
Global Const ERROR_POP_SOCKET = 22306   ' unable to create socket
Global Const ERROR_POP_CONNECT = 22307  ' unable to connect to remote host
Global Const ERROR_POP_SELECT = 22308   ' unable to select socket
Global Const ERROR_POP_TIMEOUT = 22309  ' timeout reading socket
Global Const ERROR_POP_SERVER = 22310   ' invalid POP server response
Global Const ERROR_POP_READ = 22311     ' unable to read socket
Global Const ERROR_POP_WRITE = 22312    ' unable to write socket
Global Const ERROR_POP_LOGIN = 22313    ' unable to login to server
Global Const ERROR_POP_NOLOGIN = 22314  ' not logged in to server
Global Const ERROR_POP_MESGID = 22315   ' invalid message identifier
Global Const ERROR_POP_CLOSED = 22316   ' server connection closed
Global Const ERROR_POP_BUSY = 22317     ' server is busy

'
' POP client actions
'
Global Const POP_ACTION_NONE = 0
Global Const POP_ACTION_CONNECT = 1
Global Const POP_ACTION_DISCONNECT = 2
Global Const POP_ACTION_GETHDR = 3
Global Const POP_ACTION_GETMSG = 4
Global Const POP_ACTION_DELMSG = 5
Global Const POP_ACTION_RESET = 6
Global Const POP_ACTION_CANCEL = 7

'
' POP client states
'
Global Const POP_CLIENT_UNUSED = 0
Global Const POP_CLIENT_IDLE = 1
Global Const POP_CLIENT_CONNECTING = 2
Global Const POP_CLIENT_LOGIN = 3
Global Const POP_CLIENT_TRANSFER = 4

'
' Simple Mail Transfer Protocol (SMTP) client control constants
'
' The error codes are returned by the LastError property and
' passed as an argument to the LastError event.
'
Global Const SMTP_ERROR = -1
Global Const ERROR_SMTP_HANDLE = 22201  ' invalid handle to SMTP client
Global Const ERROR_SMTP_PARAM = 22202   ' invalid paramter passed to function
Global Const ERROR_SMTP_INIT = 22203    ' unable to initialize socket library
Global Const ERROR_SMTP_ALLOC = 22204   ' unable to allocate memory
Global Const ERROR_SMTP_HOSTNAME = 22205 ' unable to resolve hostname
Global Const ERROR_SMTP_SOCKET = 22206  ' unable to create socket
Global Const ERROR_SMTP_CONNECT = 22207 ' unable to connect to remote host
Global Const ERROR_SMTP_SELECT = 22208  ' unable to select socket
Global Const ERROR_SMTP_TIMEOUT = 22209 ' timeout reading socket
Global Const ERROR_SMTP_SERVER = 22210  ' invalid SMTP server response
Global Const ERROR_SMTP_READ = 22211    ' unable to read socket
Global Const ERROR_SMTP_WRITE = 22212   ' unable to write socket
Global Const ERROR_SMTP_BUSY = 22213    ' server connection in use
Global Const ERROR_SMTP_DATA = 22214    ' server receiving message data
Global Const ERROR_SMTP_NODATA = 22215  ' no data received
Global Const ERROR_SMTP_CLOSED = 22216  ' server connection closed
Global Const ERROR_SMTP_CANCEL = 22217  ' client operation was canceled

'
' SMTP client actions
'
Global Const SMTP_ACTION_NONE = 0
Global Const SMTP_ACTION_CONNECT = 1
Global Const SMTP_ACTION_DISCONNECT = 2
Global Const SMTP_ACTION_ADDRESS = 3
Global Const SMTP_ACTION_SENDMAIL = 4
Global Const SMTP_ACTION_EXPAND = 5
Global Const SMTP_ACTION_VERIFY = 6
Global Const SMTP_ACTION_RESET = 7
Global Const SMTP_ACTION_CANCEL = 8

'
' SMTP client states
'
Global Const SMTP_CLIENT_UNUSED = 0
Global Const SMTP_CLIENT_IDLE = 1
Global Const SMTP_CLIENT_CONNECTING = 2
Global Const SMTP_CLIENT_ADDRESSING = 3
Global Const SMTP_CLIENT_COMPOSING = 4
Global Const SMTP_CLIENT_SENDING = 5

'
' Remote Access Service (RAS) control contants
'
' The error codes are returned by the LastError property and
' passed as an argument to the LastError event. These are the
' same codes returned by the RAS library, with 25000 added
' to the base value
'
Global Const ERROR_INVALID_PORT_HANDLE = 25601
Global Const ERROR_PORT_ALREADY_OPEN = 25602
Global Const ERROR_BUFFER_TOO_SMALL = 25603
Global Const ERROR_WRONG_INFO_SPECIFIED = 25604
Global Const ERROR_CANNOT_SET_PORT_INFO = 25605
Global Const ERROR_PORT_NOT_CONNECTED = 25606
Global Const ERROR_EVENT_INVALID = 25607
Global Const ERROR_DEVICE_DOES_NOT_EXIST = 25608
Global Const ERROR_DEVICETYPE_DOES_NOT_EXIST = 25609
Global Const ERROR_INVALID_BUFFER = 25610
Global Const ERROR_ROUTE_NOT_AVAILABLE = 25611
Global Const ERROR_ROUTE_NOT_ALLOCATED = 25612
Global Const ERROR_INVALID_COMPRESSION_SPECIFIED = 25613
Global Const ERROR_OUT_OF_BUFFERS = 25614
Global Const ERROR_PORT_NOT_FOUND = 25615
Global Const ERROR_ASYNC_REQUEST_PENDING = 25616
Global Const ERROR_ALREADY_DISCONNECTING = 25617
Global Const ERROR_PORT_NOT_OPEN = 25618
Global Const ERROR_PORT_DISCONNECTED = 25619
Global Const ERROR_NO_ENDPOINTS = 25620
Global Const ERROR_CANNOT_OPEN_PHONEBOOK = 25621
Global Const ERROR_CANNOT_LOAD_PHONEBOOK = 25622
Global Const ERROR_CANNOT_FIND_PHONEBOOK_ENTRY = 25623
Global Const ERROR_CANNOT_WRITE_PHONEBOOK = 25624
Global Const ERROR_CORRUPT_PHONEBOOK = 25625
Global Const ERROR_CANNOT_LOAD_STRING = 25626
Global Const ERROR_KEY_NOT_FOUND = 25627
Global Const ERROR_DISCONNECTION = 25628
Global Const ERROR_REMOTE_DISCONNECTION = 25629
Global Const ERROR_HARDWARE_FAILURE = 25630
Global Const ERROR_USER_DISCONNECTION = 25631
Global Const ERROR_INVALID_SIZE = 25632
Global Const ERROR_PORT_NOT_AVAILABLE = 25633
Global Const ERROR_CANNOT_PROJECT_CLIENT = 25634
Global Const ERROR_UNKNOWN = 25635
Global Const ERROR_WRONG_DEVICE_ATTACHED = 25636
Global Const ERROR_BAD_STRING = 25637
Global Const ERROR_REQUEST_TIMEOUT = 25638
Global Const ERROR_CANNOT_GET_LANA = 25639
Global Const ERROR_NETBIOS_ERROR = 25640
Global Const ERROR_SERVER_OUT_OF_RESOURCES = 25641
Global Const ERROR_NAME_EXISTS_ON_NET = 25642
Global Const ERROR_SERVER_GENERAL_NET_FAILURE = 25643
Global Const ERROR_AUTH_INTERNAL = 25645
Global Const ERROR_RESTRICTED_LOGON_HOURS = 25646
Global Const ERROR_ACCT_DISABLED = 25647
Global Const ERROR_PASSWD_EXPIRED = 25648
Global Const ERROR_NO_DIALIN_PERMISSION = 25649
Global Const ERROR_SERVER_NOT_RESPONDING = 25650
Global Const ERROR_FROM_DEVICE = 25651
Global Const ERROR_UNRECOGNIZED_RESPONSE = 25652
Global Const ERROR_MACRO_NOT_FOUND = 25653
Global Const ERROR_MACRO_NOT_DEFINED = 25654
Global Const ERROR_MESSAGE_MACRO_NOT_FOUND = 25655
Global Const ERROR_DEFAULTOFF_MACRO_NOT_FOUND = 25656
Global Const ERROR_FILE_COULD_NOT_BE_OPENED = 25657
Global Const ERROR_DEVICENAME_TOO_LONG = 25658
Global Const ERROR_DEVICENAME_NOT_FOUND = 25659
Global Const ERROR_NO_RESPONSES = 25660
Global Const ERROR_NO_COMMAND_FOUND = 25661
Global Const ERROR_WRONG_KEY_SPECIFIED = 25662
Global Const ERROR_UNKNOWN_DEVICE_TYPE = 25663
Global Const ERROR_ALLOCATING_MEMORY = 25664
Global Const ERROR_PORT_NOT_CONFIGURED = 25665
Global Const ERROR_DEVICE_NOT_READY = 25666
Global Const ERROR_READING_INI_FILE = 25667
Global Const ERROR_NO_CONNECTION = 25668
Global Const ERROR_BAD_USAGE_IN_INI_FILE = 25669
Global Const ERROR_READING_SECTIONNAME = 25670
Global Const ERROR_READING_DEVICETYPE = 25671
Global Const ERROR_READING_DEVICENAME = 25672
Global Const ERROR_READING_USAGE = 25673
Global Const ERROR_READING_MAXCONNECTBPS = 25674
Global Const ERROR_READING_MAXCARRIERBPS = 25675
Global Const ERROR_LINE_BUSY = 25676
Global Const ERROR_VOICE_ANSWER = 25677
Global Const ERROR_NO_ANSWER = 25678
Global Const ERROR_NO_CARRIER = 25679
Global Const ERROR_NO_DIALTONE = 25680
Global Const ERROR_IN_COMMAND = 25681
Global Const ERROR_WRITING_SECTIONNAME = 25682
Global Const ERROR_WRITING_DEVICETYPE = 25683
Global Const ERROR_WRITING_DEVICENAME = 25684
Global Const ERROR_WRITING_MAXCONNECTBPS = 25685
Global Const ERROR_WRITING_MAXCARRIERBPS = 25686
Global Const ERROR_WRITING_USAGE = 25687
Global Const ERROR_WRITING_DEFAULTOFF = 25688
Global Const ERROR_READING_DEFAULTOFF = 25689
Global Const ERROR_EMPTY_INI_FILE = 25690
Global Const ERROR_AUTHENTICATION_FAILURE = 25691
Global Const ERROR_PORT_OR_DEVICE = 25692
Global Const ERROR_NOT_BINARY_MACRO = 25693
Global Const ERROR_DCB_NOT_FOUND = 25694
Global Const ERROR_STATE_MACHINES_NOT_STARTED = 25695
Global Const ERROR_STATE_MACHINES_ALREADY_STARTED = 25696
Global Const ERROR_PARTIAL_RESPONSE_LOOPING = 25697
Global Const ERROR_UNKNOWN_RESPONSE_KEY = 25698
Global Const ERROR_RECV_BUF_FULL = 25699
Global Const ERROR_CMD_TOO_LONG = 25700
Global Const ERROR_UNSUPPORTED_BPS = 25701
Global Const ERROR_UNEXPECTED_RESPONSE = 25702
Global Const ERROR_INTERACTIVE_MODE = 25703
Global Const ERROR_BAD_CALLBACK_NUMBER = 25704
Global Const ERROR_INVALID_AUTH_STATE = 25705
Global Const ERROR_WRITING_INITBPS = 25706
Global Const ERROR_INVALID_WIN_HANDLE = 25707
Global Const ERROR_NO_PASSWORD = 25708
Global Const ERROR_NO_USERNAME = 25709
Global Const ERROR_CANNOT_START_STATE_MACHINE = 25710
Global Const ERROR_GETTING_COMMSTATE = 25711
Global Const ERROR_SETTING_COMMSTATE = 25712
Global Const ERROR_COMM_FUNCTION = 25713
Global Const ERROR_CONFIGURATION_PROBLEM = 25714
Global Const ERROR_X25_DIAGNOSTIC = 25715
Global Const ERROR_TOO_MANY_LINE_ERRORS = 25716
Global Const ERROR_OVERRUN = 25717
Global Const ERROR_ACCT_EXPIRED = 25718
Global Const ERROR_CHANGING_PASSWORD = 25719
Global Const ERROR_NO_ACTIVE_ISDN_LINES = 25720
Global Const ERROR_NO_ISDN_CHANNELS_AVAILABLE = 25721

'
' Remote command client actions
'
Global Const RCMD_ACTION_NONE = 0
Global Const RCMD_ACTION_LOGIN = 1
Global Const RCMD_ACTION_EXECUTE = 2
Global Const RCMD_ACTION_COMMAND = 3
Global Const RCMD_ACTION_DISCONNECT = 4

'
' Remote command client states
'
Global Const RCMD_CLIENT_UNUSED = 0
Global Const RCMD_CLIENT_IDLE = 1
Global Const RCMD_CLIENT_CONNECTING = 2
Global Const RCMD_CLIENT_RECEIVING = 3
Global Const RCMD_CLIENT_SENDING = 4
Global Const RCMD_CLIENT_CLOSING = 5

'
' Telnet (Network Terminal) client control constants
'
' The error codes are returned by the LastError property and
' passed as an argument to the LastError event.
'
Global Const TELNET_ERROR = -1
Global Const ERROR_TELNET_NOHANDLES = 22101   ' no free handles
Global Const ERROR_TELNET_ALLOCMEM = 22102    ' no global memory
Global Const ERROR_TELNET_NOTOWNER = 22103    ' not session owner
Global Const ERROR_TELNET_NETSTART = 22104    ' cannot initialize network
Global Const ERROR_TELNET_HOSTNAME = 22105    ' invalid hostname
Global Const ERROR_TELNET_SOCKET = 22106      ' cannot create socket
Global Const ERROR_TELNET_CONNECT = 22108     ' cannot connect to server
Global Const ERROR_TELNET_PEERNAME = 22109    ' cannot get peer information
Global Const ERROR_TELNET_PARAM = 22112       ' invalid parameter
Global Const ERROR_TELNET_HANDLE = 22113      ' invalid TELNET handle
Global Const ERROR_TELNET_SELECT = 22114      ' socket select error
Global Const ERROR_TELNET_READ = 22115        ' cannot read from socket
Global Const ERROR_TELNET_RDBUF = 22117       ' receive buffer is full
Global Const ERROR_TELNET_WRITE = 22118       ' cannot write to socket
Global Const ERROR_TELNET_WRBUF = 22119       ' send buffer is empty
Global Const ERROR_TELNET_BUSY = 22120        ' connection in use
Global Const ERROR_TELNET_NOTBUSY = 22121     ' connection is not in use
Global Const ERROR_TELNET_TIMEOUT = 22122     ' timeout waiting for socket
Global Const ERROR_TELNET_SYNC = 22123        ' internal synchronization error
Global Const ERROR_TELNET_OPTION = 22124      ' unsupported TELNET option
Global Const ERROR_TELNET_CLOSED = 22125      ' server closed connection

'
' Telnet client actions
'
Global Const TELNET_ACTION_CONNECT = 1
Global Const TELNET_ACTION_DISCONNECT = 2

'
' Telnet client states
'
Global Const TELNET_CLIENT_UNUSED = 0
Global Const TELNET_CLIENT_IDLE = 1
Global Const TELNET_CLIENT_CONNECTING = 2
Global Const TELNET_CLIENT_RECEIVING = 3
Global Const TELNET_CLIENT_SENDING = 4
Global Const TELNET_CLIENT_CLOSING = 5

'
' Terminal emulation actions
'
Global Const TERM_ACTION_CLRSCR = 1
Global Const TERM_ACTION_CLREOL = 2
Global Const TERM_ACTION_INSLINE = 3
Global Const TERM_ACTION_DELLINE = 4
Global Const TERM_ACTION_SCROLLUP = 5
Global Const TERM_ACTION_SCROLLDN = 6
Global Const TERM_ACTION_REFRESH = 7

'
' World Wide Web control contants
'
' The error codes are returned by the LastError property and
' passed as an argument to the LastError event.
'
Global Const HTTP_ERROR = -1
Global Const ERROR_HTTP_HANDLE = 22401       ' invalid handle to client
Global Const ERROR_HTTP_PARAM = 22402        ' invalid paramter
Global Const ERROR_HTTP_INIT = 22403         ' unable to initialize library
Global Const ERROR_HTTP_ALLOC = 22404        ' unable to allocate memory
Global Const ERROR_HTTP_HOSTNAME = 22405     ' unable to resolve hostname
Global Const ERROR_HTTP_SOCKET = 22406       ' unable to create socket
Global Const ERROR_HTTP_CONNECT = 22407      ' unable to connect to remote host
Global Const ERROR_HTTP_SELECT = 22408       ' unable to select socket
Global Const ERROR_HTTP_TIMEOUT = 22409      ' timeout reading socket
Global Const ERROR_HTTP_SERVER = 22410       ' invalid HTTP server response
Global Const ERROR_HTTP_NOFILE = 22411       ' cannot open local file
Global Const ERROR_HTTP_OPEN = 22412         ' cannot open local file
Global Const ERROR_HTTP_READ = 22413         ' unable to read socket
Global Const ERROR_HTTP_WRITE = 22414        ' unable to write socket
Global Const ERROR_HTTP_EOF = 22415          ' end-of-file on socket
Global Const ERROR_HTTP_BUSY = 22416         ' server is busy
Global Const ERROR_HTTP_MOVED = 22417        ' resource has been moved
Global Const ERROR_HTTP_NOTMODIFIED = 22418  ' resource has not been modified
Global Const ERROR_HTTP_BADREQUEST = 22419   ' bad request to server
Global Const ERROR_HTTP_UNAUTHORIZED = 22420 ' unauthorized request to server
Global Const ERROR_HTTP_PAYMENT = 22421      ' payment required for resource
Global Const ERROR_HTTP_FORBIDDEN = 22422    ' access to resource forbidden
Global Const ERROR_HTTP_NOTFOUND = 22423     ' resource not found on server
Global Const ERROR_HTTP_BADMETHOD = 22424    ' invalid method for this resource
Global Const ERROR_HTTP_NOTACCEPTED = 22425  ' resource not acceptable to client
Global Const ERROR_HTTP_PROXYAUTH = 22426    ' proxy authentication required
Global Const ERROR_HTTP_GONE = 22427         ' resource is permanently removed
Global Const ERROR_HTTP_INTERNAL = 22428     ' internal server error
Global Const ERROR_HTTP_NOMETHOD = 22429     ' method not supported by server
Global Const ERROR_HTTP_BADGATEWAY = 22430   ' invalid response from gateway
Global Const ERROR_HTTP_UNAVAILABLE = 22431  ' server is unavailable
Global Const ERROR_HTTP_GATETIMEOUT = 22432  ' server gateway timeout
Global Const ERROR_HTTP_UNEXPECTED = 22433   ' unexpected error

'
' Web client actions
'
Global Const WEB_ACTION_LOAD = 1
Global Const WEB_ACTION_RELOAD = 2
Global Const WEB_ACTION_UNLOAD = 3
Global Const WEB_ACTION_NEXT = 4
Global Const WEB_ACTION_PREV = 5
Global Const WEB_ACTION_CANCEL = 6
Global Const WEB_ACTION_RESET = 7
Global Const WEB_ACTION_REFRESH = 8
Global Const WEB_ACTION_SEARCH = 9
Global Const WEB_ACTION_LINEUP = 10
Global Const WEB_ACTION_LINEDOWN = 11
Global Const WEB_ACTION_PAGEUP = 12
Global Const WEB_ACTION_PAGEDOWN = 13

'
' Web client states
'
Global Const WEB_CLIENT_UNUSED = 0
Global Const WEB_CLIENT_IDLE = 1
Global Const WEB_CLIENT_CONNECT = 2
Global Const WEB_CLIENT_COMMAND = 3
Global Const WEB_CLIENT_LOAD = 4
Global Const WEB_CLIENT_UNLOAD = 5

'
' Web client resource types
'
Global Const WEB_RESOURCE_UNKNOWN = 0
Global Const WEB_RESOURCE_HTML = 1
Global Const WEB_RESOURCE_TEXT = 2
Global Const WEB_RESOURCE_IMAGE = 3

'
' SocketWrench error response values
'
Global Const SOCKET_ERRIGNORE = 0
Global Const SOCKET_ERRDISPLAY = 1

'
' SocketWrench control actions
'
Global Const SOCKET_OPEN = 1
Global Const SOCKET_CONNECT = 2
Global Const SOCKET_LISTEN = 3
Global Const SOCKET_ACCEPT = 4
Global Const SOCKET_CANCEL = 5
Global Const SOCKET_FLUSH = 6
Global Const SOCKET_CLOSE = 7
Global Const SOCKET_DISCONNECT = 7
Global Const SOCKET_ABORT = 8

'
' SocketWrench control states
'
Global Const SOCKET_NONE = 0
Global Const SOCKET_IDLE = 1
Global Const SOCKET_LISTENING = 2
Global Const SOCKET_CONNECTING = 3
Global Const SOCKET_ACCEPTING = 4
Global Const SOCKET_RECEIVING = 5
Global Const SOCKET_SENDING = 6
Global Const SOCKET_CLOSING = 7

'
' Address families
'
Global Const AF_UNSPEC = 0
Global Const AF_UNIX = 1
Global Const AF_INET = 2

'
' Socket types
'
Global Const SOCK_STREAM = 1
Global Const SOCK_DGRAM = 2
Global Const SOCK_RAW = 3
Global Const SOCK_RDM = 4
Global Const SOCK_SEQPACKET = 5

'
' Protocol types
'
Global Const IPPROTO_IP = 0
Global Const IPPROTO_ICMP = 1
Global Const IPPROTO_GGP = 2
Global Const IPPROTO_TCP = 6
Global Const IPPROTO_PUP = 12
Global Const IPPROTO_UDP = 17
Global Const IPPROTO_IDP = 22
Global Const IPPROTO_ND = 77
Global Const IPPROTO_RAW = 255
Global Const IPPROTO_MAX = 256

'
' Common ports
'
Global Const IPPORT_ANY = 0
Global Const IPPORT_ECHO = 7
Global Const IPPORT_DISCARD = 9
Global Const IPPORT_SYSTAT = 11
Global Const IPPORT_DAYTIME = 13
Global Const IPPORT_NETSTAT = 15
Global Const IPPORT_FTP = 21
Global Const IPPORT_TELNET = 23
Global Const IPPORT_SMTP = 25
Global Const IPPORT_TIMESERVER = 37
Global Const IPPORT_NAMESERVER = 42
Global Const IPPORT_WHOIS = 43
Global Const IPPORT_MTP = 57
Global Const IPPORT_FINGER = 79
Global Const IPPORT_HTTP = 80
Global Const IPPORT_TFTP = 69
Global Const IPPORT_RESERVED = 1024
Global Const IPPORT_USERRESERVED = 5000

'
' Network addresses
'
Global Const INADDR_ANY = "0.0.0.0"
Global Const INADDR_LOOPBACK = "127.0.0.1"
Global Const INADDR_NONE = "255.255.255.255"

'
' Shutdown values
'
Global Const SOCKET_READ = 0
Global Const SOCKET_WRITE = 1
Global Const SOCKET_READWRITE = 2

'
' Declarations for functions to encode and decode files, typically
' used as with attachments to mail messages or news articles
'
Declare Function DecodeFile Lib "UUCODE16.DLL" (ByVal InputFile As String) As Integer
Declare Function EncodeFile Lib "UUCODE16.DLL" (ByVal InputFile As String, ByVal OutputFile As String) As Integer
Declare Function DecodeBase64File Lib "UUCODE16.DLL" (ByVal InputFile As String, ByVal OutputFile As String) As Integer
Declare Function EncodeBase64File Lib "UUCODE16.DLL" (ByVal InputFile As String, ByVal OutputFile As String) As Integer

