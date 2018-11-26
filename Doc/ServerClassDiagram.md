@startuml'/继承
BaseSocketConnection <|-- TcpServer
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
class BaseMessageDisptcher
MessageRoute --> BaseMessageDisptcher
@enduml

'/继承
@startuml
BaseMessageDisptcher <|-- ServerMessageDispatcher
@enduml

'/聚合
@startuml
BaseMessageDisptcher o-- BaseProcessor
@enduml



'/实现
@startuml
abstract class BaseProcessor
interface BaseProcessor
BaseProcessor <|.. MoveProcessor
BaseProcessor <|.. SigninProcessor
@enduml