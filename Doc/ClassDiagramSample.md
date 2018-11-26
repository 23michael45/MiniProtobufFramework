@startuml'/访问权限控制
class Dummy {
  - private field1
  # protected field2
  ~ package method1()
  + public method2()
}
@enduml

'/继承
@startuml
Father <|-- Son
@enduml

'/实现
@startuml
abstract class AbstractList
interface List
List <|.. AbstractList
@enduml

'/依赖
@startuml
Human ..> Cigarette
@enduml

'/关联
@startuml
class Water
class Human
Human --> Water
@enduml

'/聚合
@startuml
Company o-- Human
@enduml

'/组合
@startuml
Human *-- Brain
@enduml 