# SSDProject# SSDProject
## 팀명: CCC (Clean Code Collective)
# 팀장: 김범진 / 팀원: 김경민, 김윤진, 김율곤, 정지윤

# Ground Rule
1. Commit message에 반드시 prefix를 사용한다
   [Git]
   [TDD] [TDD_Red] [TDD_Green] [TDD_Refactor]
   [Feature] [UT or Test] [Refactor] [BugFix]
2. 업무는 Issue를 생성하여 관리한다
3. PR 생성시 PR template을 성실하게 채운다
4. Merge는 approve 2인 이상을 필수로 한다
5. 리뷰 코멘트는 인당 1개 이상을 목표로 한다
6. 하루 1회 이상 healthy tag 확보한다 (아침 출근 / 점심 식사후)
7. Healthy tag에 code coverage를 기입한다


<img width="1532" height="875" alt="image" src="https://github.com/user-attachments/assets/38e8ec10-8265-44ee-b959-8d7c241ffd6a" />



- 임시 plantump layered Arch. 적용


@startuml
skinparam componentStyle rectangle
skinparam shadowing false

package "test_shell.exe" {

  package "Shell Layer" {
    component Shell  
    component Runner  
  }

  package "Command Parser Layer" {
    component CommandParser
  }

  package "Executor Layer" {
    interface IExecutor
    component Writer
    component Reader
    component Exiter
    component Helper
    component Eraser
    component Comparer
    component Flusher
    component CompositeExecutor
    component FullWriteAndReadCompare
    component PartialLBAWrite
    component WriteReadAging
    component EraseAndWriteAging
  }

  package "SSD App Layer" {
    component SSDAppController
    component LBAManager
  }
}

package "SSD.exe" {

  package "HostInterface Layer" {
    interface IProcessor
    component WriteProcessor
    component ReadProcessor
    component EraseProcessor
    component HostInterface
  }

  package "SSD Layer" {
    component SSD
    interface NandInterface
    component Nand
    component BufferManager
  }

  ' 내부 클래스 관계
  SSD --> NandInterface
  Nand -up-|> NandInterface
  HostInterface --> IProcessor

  ' Processor 계층 관계
  WriteProcessor ..|> IProcessor
  ReadProcessor ..|> IProcessor
  EraseProcessor ..|> IProcessor

  IProcessor --> BufferManager
  IProcessor --> SSD
}

' 실행파일 간 계층 의존 관계
Shell --> CommandParser
Runner --> CommandParser
CommandParser --> IExecutor

Writer ..|> IExecutor
Reader ..|> IExecutor
Eraser ..|> IExecutor
Exiter ..|> IExecutor
Helper ..|> IExecutor
Comparer ..|> IExecutor
Flusher ..|> IExecutor

CompositeExecutor ..|> IExecutor
FullWriteAndReadCompare ..|> CompositeExecutor
PartialLBAWrite ..|> CompositeExecutor
WriteReadAging ..|> CompositeExecutor
EraseAndWriteAging ..|> CompositeExecutor

IExecutor --> SSDAppController
SSDAppController --> HostInterface
SSDAppController --> LBAManager

@enduml
