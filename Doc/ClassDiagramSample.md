@startuml
class Dummy {
  - private field1
  # protected field2
  ~ package method1()
  + public method2()
}
@enduml

@startuml
Father <|-- Son
@enduml

@startuml
abstract class AbstractList
interface List
List <|.. AbstractList
@enduml

@startuml
Human ..> Cigarette
@enduml


@startuml
class Water
class Human
Human --> Water
@enduml

@startuml
Company o-- Human
@enduml

@startuml
Human *-- Brain
@enduml 