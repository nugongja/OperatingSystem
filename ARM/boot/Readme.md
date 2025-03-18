# Assembly Instructions
1. MRS (Move from Status Register)
MRS r0, cpsr
- cpsr의 값을 r0레지스터로 이동
- 즉, 현재 cpsr값을 r0에 저장하여 이후에 수정할 수 있도록 함

2. BIC (Bit Clear)
BIT r1, r0, #0x1F
- BIC (Bit Clear): 특정 비트를 0으로 만듦
- r0의 값을 기반으로 r1을 생성하되, 하위 5비트(0x1F)를 0으로 만듦
- 이 작업은 현재 CPSR의 동작 모드를 변경하기 위해 기존 모드 비트를 지우는 과정임.

3. ORR (Logical OR)
ORR r1, r1, #ARM_MODE_BIT_SVC
- ORR (Logical OR): 특정 비트를 1로 설정
- r1의 하위 5비트(Mode 비트)를 Supervisor Mode(SVC, 0x13)로 설정
- 즉, BIC로 모드 비트를 클리어한 후, ORR로 새로운 모드 값을 설정하는 과정

4. MSR (Move to Status Register)
MSR cpsr, r1
- cpsr에 새로운 값 r1을 설정하여 CPU의 동작 모드를 변경
- 즉, 현재 동작 모드를 SVC 모드(Supervisor Mode)로 변경

5. LDR (Load Register)
LDR sp, =SVC_STACK_TOP
- sp (스택 포인터)에 SVC_STACK_TOP의 주소를 로드
- 즉, Supervisor Mode의 스택을 설정하는 과정

6. BL (Branch)
BL main
- main(점프 대상)으로 이동
- BL로 점프를 하려면 점프 대상 레이블이 같은 파일 안에 있어야 한다. 
- 다른 파일에 있다면 링커라 링킹할 수 있도록 레이블을 .global로 선언해야 한다. 
    - 컴파일러는 C언어 함수 이름을 링커가 자동으로 접근할 수 있는 전역 심벌로 만든다. 
    - 전역 심벌은 어셈블리어를 쓸 때 .global지시어로 선언하는 이름이고 C언어를 쓸 때 extern으로 선언하는 이름이다. 
