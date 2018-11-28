@startuml'/继承
BaseSocketConnection <|-- TcpClient
@enduml

'/关联
@startuml
class BaseSocketConnection
class MessageRoute
BaseSocketConnection <--> MessageRoute
@enduml

'/关联
@startuml
class MessageRoute
class BaseMessageDispatcher
MessageRoute --> BaseMessageDispatcher
@enduml

'/继承
@startuml
BaseMessageDispatcher <|-- ClientMessageDispatcher
@enduml

'/聚合
@startuml
BaseMessageDispatcher o-- BaseProcessor
@enduml



'/实现
@startuml
abstract class BaseProcessor
interface BaseProcessor
BaseProcessor <|.. MoveProcessor
BaseProcessor <|.. SigninProcessor
@enduml