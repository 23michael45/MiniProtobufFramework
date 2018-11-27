@startuml
class 数据包统一结构 
{
  - pbh\0   	  4字节							头标识
  # total length  4字节  						数据包总长，从本字节开始计算 不算头标识 即 4 + 4 + 包头长 + 包体长 
  ~ head length	  4字节  						包头长度
  + CmdType Data  head length字节   			为Protobuf结构  包头信息CmdType  包括包体类型与包体长度  
  - Proto Data    total - head - 4 - 4字节	    为Protobuf结构  包体信息，可按抱头中的类型解包  
}
@enduml

@startuml
Human ..> Cigarette
Human ..> 1
Human ..> 2
@enduml

