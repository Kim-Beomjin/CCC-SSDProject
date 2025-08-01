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


<img width="1033" height="628" alt="image" src="https://github.com/user-attachments/assets/2d82e2ee-f820-4194-9f32-56e357d186b6" />




- 임시 plantump layered Arch. 적용
@startuml
skinparam componentStyle rectangle
skinparam shadowing false

package "UserLayer" {
  [Shell]  
  [Runner]  
}

package "Command Parser Layer" {
  [CommandParser]
}

package "Executor Layer" {
  [IExecutor]  
  [Writer]
  [Reader]
  [Eraser]
  [Comparer]
  [Flusher]
}

package "SSD App Layer" {
  [SSDAppController]
  [LBAManager]
}

package "SSD HW Layer" {
  [SSDHardwareInterface]
  [FlashDevice]
}

' 의존 관계
[Shell] --> [CommandParser]
[Runner] --> [CommandParser]
[CommandParser] --> [IExecutor]

[Writer] -up-|> [IExecutor]
[Reader] -up-|> [IExecutor]
[Eraser] -up-|> [IExecutor]
[Comparer] -up-|> [IExecutor]
[Flusher] -up-|> [IExecutor]

[IExecutor] --> [SSDAppController]
[SSDAppController] --> [SSDHardwareInterface]
[SSDAppController] --> [LBAManager]
[LBAManager] --> [FlashDevice]
[SSDHardwareInterface] --> [FlashDevice]

@enduml
