# chapter 3

## 1. 리셋 벡터
ARM코어에 전원이 들어가면 ARM 코어가 가장 먼저 하는 일 -> 리셋 벡터에 있는 명령어를 실행함
- 리셋 벡터 :메모리 주소 0x00000000
- 0x00000000에서 32비트를 읽어서 그 명령어를 실행함
- .text : text섹션
- .code 32 : 명령어의 크기가 32bit
- .global : C언어에서의 extern과 같음 -> 변수의 정보를 외부 파일에서 심벌로 읽을 수 있음
</br>

## 2. 컴파일
- arm-none-eabi-as ~ : arm-none-eabi 컴파러일러의 소스 파일 컴파일 명령어
    - -march=armv7-a : 아키텍쳐
    - -mcpu=cortex-a8 : RealViewPB(emulator)가 사용하는 ARM코어가 cortex-a8임
    - **.o파일이 나옴** -> ELF(Executable and Linkable Format)형식임
    - **ELF(Executable and Linkable Format) 란?** : 리눅스 기반 시스템의 기본 바이너리 형식임
        - 실행 가능한 바이너리 파일, 목적 파일, 공유 라이브러리, 코어 덤프 등에서 사용되는 형식이다.  
        - 실행 가능한 바이너리 코드뿐만 아니라 심볼 테이블, 디버깅 정보, 섹션 헤더 등의 추가적인 메타데이터를 포함하는 파일 형식임
- arm-none-ebai-objcopy : 바이너리만 뽑아냄
    - .o파일은 심벌 정보 등이 포함되어있음. 하지만 임베디드 환경에서는 이러한ELF 형식이 필요하지 않고, 순수한 바이너리 코드만 필요할 수 있음 
    - objcopy를 사용하면 필요한 부분만 추출 가능 
    - 바이너리만 뽑아내려면 -O binary옵션으로 바이너리를 카피해야 함
    - hexdump 명령어로 바이너리 확인 가능
</br>

## 3. 실행 파일 만들기
- ELF파일을 만들려면 링커의 도움이 필요하다. 
- **linker** :  여러 오브젝트 파일을 묶어서 하나의 실행 파일로 만드는 프로그램
    - 링커가 동작하려면 링커에 정보를 던져주는 파일이 필요한데, 그걸 **링커 스크립트**라고 함. (.ld)
    - 일반적인 애플리케이션 개발에서는 링커를 신경쓰지 않음
        - 운영체제가 자동으로 메모리를 관리함
        - 링커가 자동으로 적절한 설정을 사용함
        - OS의 Loader가 실행 시점에 필요한 라이브러리를 적절한 메모리에 배치하고 연결해줌
        => 즉, 개발자가 직접 링커 스크립트를 수정할 필요가 없음 
    - 임베디드 환경에서는 운영체제가 없거나 제한적이므로, 코드와 데이터를 정확한 메모리 위치에 배치해야 함
        - 메모리 배치를 수동으로 설정해야
        - RAM이 제한적이므로 스택과 힙을함 최적 배치해야함
- arm-none-eabi-ld : 실행 파일 만드는 명령어(.ld와 .o를 사용)
    - -n : 링커에 섹션의 정렬을 자동으로 맞추지 말라고 지시하는 옵 션  
    - -T : 링커 스크립트의 파일명을 알려주는 옵션
    - -nostdlib : 링커가 자동으로 표준 라이브러리를 링킹하지 못하도록 짓하는 옵션
    - **링커가 동작을 완료하면 .axf파일이 생성됨**
</br>

## 4. 실행하기
- ./navilos.axf 하면 Format error : 리눅스 커널에서 동작하지 않는 섹션 배치로 만들어져있기 때문 
- 실행 방법 1 : ARM 개발 보드에 다운로드해서 동작을 확인
- 실행 방법 2: QEMU로 실행
    - qemu-system-arm -M realview-pb-a8 -kernel navilos.axf -S -gdb tcp::1234
        - -S -gdb tcp::1234 -> 옵션은 gdb와 연결하는 소켓 포트를 지정하는 옵션
        - qemu로 실행하면 화면에 아무것도 나오지 않기 때문에 오류라고 착각할 수 있음 -> gdb를 통해 메모리를 확인해야함
        - arm-none-eabi-gdb 를 설치하면 gdb-multiarch가 대신 다운로드됨 -> 아마 arm-none-eabi-gdb는 더이상 안쓰이는 듯?
            - gdb-multiarch 실행 후, target remote localhost:소켓번호
        - x/4x 0 : 0x00000000 메모리 주소에서 4바이트 출력
            - 결과가 나오면 navilos.axf파일에 있는 코드 데이터가 QEMU의 메모리로 제대로 다운로드 되었다는 뜻!
</br>

## ❤️  빌드 자동화하기
실행 파일을 만들기 위해서는 arm-none-eabi-as로 컴파일 한 뒤, arm-none-eabi-ld로 링킹을 해야 함
- 만약 어셈블리 파일이 하나 더 생기면 arm-none-eabi-as를 한 번 더 한 뒤, arm-none-eabi-ld에 입력 파일명을 추가해 줘야하기 때문에 매우 귀찮음
**=> Makefile을 만들어서 빌드 자동화!**
</br>

## ❤  하드웨어 정보 읽어오기 - 데이터시트
하드웨어와 상호작용 한다 = 하드웨어에서 정보를 일고오고, 하드웨어에 정보를 쓰는 작업을 한다.
-> 레지스터를 이용!: 하드웨어가 소프트웨어와 상호작용하는 인터페이스
- 펌웨어를 개발할 때, 해당 하드웨어의 레지스터 사용법을 알아야 하고, 하드웨어의 레지스터 사용법은 데이터 시트에 나와있음. 
    - **데이터 시트**: 해당 하드웨어가 가지고 있는 레지스터의 목록과 설명, 그리고 레지스터에 어떤 값을 썼을 때 하드웨어가 어떻게 동작하는지를 적어 놓은 문서.
    - 레지스터에 대한 예제를 제공하는 **Application Note**라는 것도 있음. 

----------------------------------------------------------------------------------------
# chapter 4 (부팅하기)
**팜웨어에서의 부팅** : 시스템에 전원이 들어가서 모든 초기화 작업을 마치고 펌웨어가 idle상태가 되 때까지를 말하거나, 시>스템에 전원이 들어가고 ARM 코어가 리셋 익셉션 핸들러를 모두 처리한 다음에 본격적으로 C언어 코드로 넘어가기 직전을 말함.

## 1. 메모리 설계 
실행파일은 메모리를 크게 세 가지로 나누어 사용한다
- text영역 : 코드가 있는 공간. 코드이므로 임의로 변경하면 안된다.
- data영역 : 초기화한 전역 변수가 있는 공간. 전역 변수를 선언할 때 초기값을 할당해서 선언하면 해당 전역 변수가 점유하는 공간은 여기에 할당됨.
- BSS영역 : 초기화하지 않은 전역 변수가 있는 공간. 초기화하지 않은 전역변수이므로 빌드 완료되어 생성된 바이너리 파일에는 심벌과 크기만 들어있음.
+ Heap : malloc() 또는 new를 통해 동적으로 할당하는 메모리
+ Stack : 함수 호출 시 지역 변수 및 리턴 주소 저장(task stack) 
- 속도는 빠르지만 작은 메모리와 속도는 느리지만 용량이 큰 메모리가 있다면, text와 일부 속도에 민감한 data영역의 데이터들은 전자에, 나머지 data와 bss의 데이터들은 후자에 배치
- 특히 .data, .bss같은 메모리 영역은 어디에서 사용되는가가 중요하다.
    - 즉, 이 변수들이 사용자 모드(USR)에서 사용되는 것인지, 커널 모드(SVC)에서 사용되는 것인지에 따라 동작 방식이 달라질 수 있다.
    - 예를 들어,
        int global_var = 10; 같은 변수가 .data 영역에 할당되더라도,
        **사용자 프로세스(USR 모드)**에서 실행되면 사용자 영역의 .data를 사용한다.
        **커널 모드(SVC 모드)**에서 실행되면 커널 전용 .data 영역이 따로 할당된다.
    - **ARM 프로세서의 실행 모드** (https://velog.io/@mysprtlty/ARM%EC%9D%98-%EB%8F%99%EC%9E%91-%EB%AA%A8%EB%93%9C)
</br>

## 2. 익셉션 벡터 테이블 만들기
### ⭐ **Exception Vector Table**: CPU에서 Exceptions나 Interrupts가 발생했을 때, 해당 예외 처리를 할 Handler주소를 저장하는 테이블이다. (예외 발생시 실행될 코드의 시작 주소를 저장해 놓은 공간)
- 전원이 켜지면 ARM은 익셉션 벡터의 리셋 벡터를 읽는다. 
    - 대부분은 ARM이 정한 기본 위치인 0x00000000 메모리 주소에 익셉션 벡터 테이블을 배치함.
- 익셉션은 각각 4바이트씩 -> 익셉션 핸들러로 이동하는 브랜치 명령어를 작성
- exception이 발생하면 PC를 익셉션 벡터 테이블의 익셥센 벡터 오프셋으로 변경 -> 브랜치 명령에 따라 익셉션 핸들러로 진입 -> 익셉션 처리 -> 예외 처리 전 위치로 복귀
    - 익셉션이 발생하면 프로그램 흐름이 다시 원래 위치로 복귀할 수 있도록 ARM에서는 R14(LR)에 복귀할 주소를 자동으로 저장함.
    - 정확히는 예외 처리 다음에 실행할 명령어 위치 **(PC + 4)**
- **CPSR, SPSR**
    - CPSR(Current Program Status Register) : 현재 CPU의 상태를 나타내는 레지스터로, 프로세서의 실행 모드, 인터럽트 상태, 조건 플래그 등을 포함한다.
        - 32비트
        -ex) 31  30  29  28  27       8  7   6   5   4  3  2  1  0</br>
             [N] [Z] [C] [V] [reserved] [I] [F] [T] [M4 M3 M2 M1 M0]
            - M4~M0에서 실행모드를 결정 (USR: 10000, FIQ: 10001, ...) 
    - SPSR(Saved Program Status Register) : 예외 또는 인터럽트가 발생했을 때 CPSR의 값을 저장하는 레지스터이며, 나중에 예외 처리가 끝난 후 원래 상태로 복귀하기 위해 필요함.
        - SPSR이 동작하는 방법
            1. 예외 발생
                - ARM CPU가 예외를 감지하면 현재 실행모드를 변경하고 CPSR을 SPSR에 저장 (R14에 PC+4도 저장함) 
                - 예외 핸들러가 실행
            2. 예외 처리 후 복귀
                - 예외 처리가 끝나면 MOVS 또는 SUBS 명령을 사용하여 SPSR값을 CPSR로 복구.
                - R14 -> PC로 복구.
                - 원래 실행 상태로 돌아감

</br>

### ⭐ 인터럽트 
: 외부 요인에 의해서 발생하는 것!
- 인터럽트는 필연적으로 인터럽트 지연을 발생시킨다. 인터럽트 지연이란, 하드웨어가 인터럽트를 감지해서 ARM에 인터럽트 신호가 입력되는 순간부터 펌웨어에서 인터럽트 핸들러가 수행되기 직전까지 걸리는 시간이다. 
    - 매우 짧은 시간이지만 임베디드 시스템의 목적에 따라서 이 시간이 문제가 되기도 한다. </br>
    => 그래서 벡터 인터럽트 컨트롤러(Vector Interrupt Controller, VIC)등의 별도 하드웨어로 인터럽트 지연을 최대한 줄이려는 여러 시도가 존재함 </br>
- ARM은 두 종류의 인터럽트를 제공한다. IRQ, FIQ (그 외에도 NMFI, LIL, IC 등이 존재)</br>
#### 1. IRQ(Interrupt Request)
- IRQ는 FIQ보다 우선순위가 낮으므로 만약 IRQ와 FIQ가 동시에 발생하면 ARM은 FIQ에 대한 처리 요청을 팜웨어에 먼저 보낸다.
- CPSR의 I비트를 1로 설정하면 IRQ익셉션을 비활성화함 (비활성화하면 자동으로 IRQ에 해당하는 모든 인터럽트 요청도 펌웨어로 처리되지 않음)
    
#### 2. FIQ(Fast Interrupt Request)

#### 3. NMFI(None-Maskable Fast Interrupt)
- NMFI를 켜면 FIQ를 비활성화 할 수 없다. 하드웨어가 자동으로 CPSR의 F비트를 0으로 클리어한다. 

#### 4. LIL(Low Interrupt Latency)
- 인터럽트 지연을 줄이기 위한 기능 중 하나로 ARM의 기본 설정 기능이다. 
- LIL은 인터럽트가 발생했을 때 현재 수행중인 명령의 실행이 아직 끝나지 않았더라도 실행 중인 명령어를 취소해버리고 인터럽트를 먼저 처리한다.
- 실행이 끝나지 않은 명령어는 인터럽트 핸들러의 처리가 모두 끝난 다음에 원래 프로그램의 진행 흐름으로 복귀할 때 인터럽트가 발생했던 시점에서 한 명령어 뒤로 다시 돌아가도록 해서 처리한다. -> 같은 명령어를 두 번 실행한다는 뜻이지만, 인터럽트의 지연을 줄일 수 있다.
- ARM은 메모리 타입을 strongly ordered로 설정할 수 있다. Strongly ordered로 설정된 메모리나 장치는 읽기, 쓰기, 동작을 수행한 순서와 횟수를 보장해야하기 때문에, 이 값으로 설정된 메모리나 장치는 메모리 접근을 시작한 후에는 수행이 끝날 때까지 중간에 실행을 멈출 수 없다.  따라서 인터럽트 지연 시간을 최소화하기 위해서는 strongly ordered로 설정된 메모리나 장치에 대해서는 multiword load/store 명령을 되도록 사용하지 않는게 좋음.

#### 5. VIC(Vectored Interrupt Controller)
- VIC를 포함하는 Interrupt Controller는 인터럽트 처리를 전담하는 일종의 주변장치이다. 
- **인터럽트 컨트롤러가 하는 일**      
    - 인터럽트가 발생했을 때 해당 인터럽트의 종류를 레지스터에 기록한다.
    - 인터럽트 발생 시, ARM의 IRQ 혹은 FIQ에 맞춰서 인터럽트 신호를 준다.
    - 특정 인터럽트를 골라서 마스킹할 수 있다.
    - 여러 인터럽트 간에 우선순위를 정할 수 있다.
- VIC의 장점 
    - VIC를 사용하면 다음과 같은 세 단계를 하드웨어가 모두 처리한 다음 곧바로 팜웨어의 인터럽트 서비스 루틴으로 진입한다. (일반적으로는 펌웨어가 세 단계를 처리한 뒤 인터럽트 서비스 루틴으로 진입함)
    - VIC가 해당 일을 할 때 펌웨어가 다른 작업을 할 수 있기 때문에 좋음.
    1. 인터럽트 컨트롤러에서 인터럽트 소스가 어떤 것인지를 판별한다.
    2. 인터럽트 소스에 따라 실행해야 할 인터럽트 서비스 루틴을 선택한다. (인터럽트 서비스 루틴 != 인터럽트 핸들러 -> 인터럽트 서비스 루틴은 인터럽트 핸들러의 하위 개념!)
    3. 해당 인터럽트 소스를 비활성화하고 인터럽트 서비스 루틴으로 진입한다. 
- VIC는 인터럽트에 대해서 인터럽트 서비스 루틴의 시작 메모리 주소를 직접 저장한다. 물론 주소는 펌웨어가 지정함.
    -> 인터럽트가 발생하면 VIC에 의해서 인터럽트 서비스 루틴으로 PC값이 바뀐다. 

</br>

### ⭐ Abort
: 어디서 문제가 일어났는지 보고하지 않고 프로그램의 동작이 더 이상 진행되지 않도록 하는 것.
- Interrupt는 데이터 처리를 위해서 정상적인 프로그램의 흐름을 끊는 익셉션이라면, Abort는 비정상적인 동작으로 인해 정상적인 프로그램의 진행을 더 이상 진행할 수 없을 때 발생하는 익셉션이다.
- ARM에서 abort가 발생하는 경우
    1. MPU로 보호되는 메모리 영역에 접그 권한 없이 접근했을 때
    2. AMBA메모리 버스가 에러를 응답했을 때
    3. ECC로직에서 에러가 발생했을 때
    - 주로 메모리와 관련된 익셉션임을 알 수 있다. ARM이 메모리에서 어떤 정보를 읽는 경우는 딱 두 가지인데, 하나는 명령어를 읽는 것과, 다른 하나는 데이터를 읽는 것이다.
        - 명령어를 읽을 때 abort가 발생하면, prefetch abort익셉션으로 진행하고
        - 데이터를 읽을 때 abort가 발생하면, data abort익셉션으로 진행한다. 
        - 메모리나 다른 것들은 정상인데 읽어온 명령어가 ARM이 모르는 명령어라면, undefined instructon익셉션을 발생시키고, 이것 또한 일종의 abort이다. 
</br>

### ⭐ Operating mode & Banked register 

#### \<Operating Mode>
    1. USR
    - 일반적으로 작동하는 모드로 ARM상태와 Thumb상태로 동작한다. 만약 운영체제를 사용한다면 사용자 프로그램은 일반적으로 USR모드로 동작한다.
    
    2. FIQ
    - FIQ익셉션이 발생하면 FIQ모드로 전환된다. FIQ모드는 ARM상태일 때에만 동작한다. 
    - 빠른 동작을 위해 별도의 레지스터를 몇 개 더 가지고 있다. -> banked register (R8_fiq~R12_fiq)
    
    3. IRQ
    - IRQ익셉션이 발생하면 IRQ모드로 전환된다. 
    - IRQ모드는 ARM상태일 때와 Thumb상태일 때 모두 동작 가능하다.
    
    4. SVC
    - 운영체제 등에서 시스템 코들르 수행하기 위한 보호 모드이다. 
    - 보통 운영체제에서 system call을 호출하면 SVC익셉션을 발생시켜 SVC모드로 전환한 후에 커널 동작을 수행한다. 
    
    5. ABT
    - Data abort나 Prefetch abort가 발생했을 때 전환되는 동작 모드 
    
    6. SYS
    - 운영체제 등에서 사용자 프로세스가 임시로 커널 모드를 획득해야 하는 경우에 사용한다. 
    - SYS모드는 익셉션과 연관되어 있지 않고 소프트웨어로 모드 전환을 하여 진입할 수 있다. 
    
    7. UND
    - Undefined instruction이 발생했을 때 진입하는 동작 모드이다. 

#### \<register>
**각 동작 모드에 따라 ARM은 각기 다른 레지스터를 가지고 있기도 하고, 각 모드들이 같은 레지스터를 공유하기도 한다.**
- ARM은 작업 레지스터를 총 37개 가지고 있음 
    - 공유 레지스터 (R0~R7, R8~R12(FIQ는 안씀), R15(PC), CPSR) => 14개 
    - bancked register (R13_x(SP), R14_x(LR), R8_fiq~R12_fiq, SPSR_x(SYS,USR제외) => 17개
    - 범용 레지스터: R0~R12까지. 펌웨어가 데이터를 일반적으로 처리할 때 사용하는 레지스터이다. 
    - Stack Poiner : R13. 대부분의 소프트웨어는 스태을 기반으로 동작하므로 항상 스택의 위치를 알아야 한다.
    - Link Register : R14. 대부분의 소프트웨어는서브 루틴 호출 혹은 함수 호출로 이루어져있음. 리턴 어드레스를 저장하는 레지스터가 R14이다.
    - Program Counter : R15. 현재 명령어를 실행하고 그 다음 명령어를 읽으려면 그다음 명령어가 있는 메모리 주소를 저장하고 있어야 한다. 
        - ARM모드일 때는 명령어 한개가 처리되고 나면 PC+4
        - Thumb모드일 때는 명령어 한개가 처리되고 나면 PC+2 
    - Bancked Register : 각 동작 모드에서 공유하지 않고 전용으로 사용하는 레지스터는 같은 이름을 가지고 있지만, 실제로 독립된 공간에 데이터를 저장한다. 이런 레지스터를 Bancked Register라고 부른다. 
        - 각 동작 모드별로 SP와 LR 그리고 SPSR이 뱅크드 레지스터이다.
 
</br>

### ⭐ ARM모드와 Thumb모드
ARM은 두 종류의 명령어 집합을 가지고 있다. 
- ARM 모드 : 32bit 명령어 집합 
- Thumb 모드 : 16bit 명령어 집합 
    - 크기가 ARM모드의 반이어야 할 것 같지만 실제로는 ARM모드에 비해서 70% 작다. 
    - ARM 모드보다 느리다. 
    - SP, LR, PC를 제외한 R8이상의 높은 레지스터의 사용이 제한된다. 
- ARM은 ARM모드와 Thumb모드를 하나의 팜웨어에서 섞어 쓰는 것을 허용한다.
=> 속도가 별로 중요하지 않은 부분은 Thumb모드로 컴파일해서 링크하고, 속도가 중요한 부분은 ARm 모드로 컴파일해서 링크한다.

</br>

### ⭐ 프로그램 상태 레지스터(Program Status Register, PSR)
- CPSR : 현재 상태를 저장하는 레지스터
- SPSR : 상태를 저장하는 레지스터 
    - SPSR는 CPSR의 백업이다. 따라서 CPSR과 SPSR은 구조가 똑같다. 
    - 당연히 32bit
  
</br>

## 3. 익셉션 핸들러 만들기
**가장 먼저 만들어야 할 익셉션 핸들러는 "리셋 익셉션 핸들러"!! -> 메모리 맵을 설정해주는 작업** 
- 각 동작 모드별 스택 주소를 각 동작 모드의 뱅크드 레지서트 SP에 설정하는 작업이다. 
- 동작 모드별 스택이 모두 설정되고 나면, C언어 main()함수로 진입한다. 그 후부터는 어셈블리어가 아닌 C언어로 임베디드 시스템을 제어할 수 있다.

-----------------------------------------------------------------
# chapter 5

## 1. UART
UART란, 병렬 데이터의 형태를 직렬 방식으로 전환하여 데이터를 전송하는 컴퓨터 하드웨어의 일종이다. (QEMU를 사용하는 환경에서 쓸 수 있는 하드웨어는 제한되어있다. 실습에서 사용하는 RealViewPB에는 PL011이라는 UART하드웨어 모듈이 붙어있으며, ARM 인포센터에 이 하드웨어의 데이터시트가 있다.) 
- 직렬(seriral) 통신: 1개의 입출력 핀을 통해 8개의 비트를 한번에 전송하는 방법
    - 0 0 0 0 1 1 0 1   ->   0 0 0 0 1 1 0 1 차례로 전용
- 병렬(parallel) 통신: n비트의 데이터 전송을 위해 n개의 입출력 핀을 사용하는 방법
    0 -> 0</br> 
    0 -> 0</br> 
    0 -> 0</br> 
    0 -> 0</br>  
    1 -> 1</br> 
    1 -> 1</br> 
    0 -> 0</br>
    1 -> 1</br>

- MCU에서는 흔히 직렬 통신 방식이며, 그 중 많이 쓰이는 방법 중 하나가 UART이다. 
- 참고 (https://shek.tistory.com/41)
- UART는 보통 콘솔 입출력용으로 사용한다. 
- 문자 통신용으로 만든 프로토콜이 아니며, 어떤 데이터 값이든 주고받을 수 있다. 
 
</br>

## 1.1 UART 공용 인터페이스 
- PL011은 RealViewPB의 UART이다. 다른 플랫폼은 다른 종류의 UART하드웨어를 가지고 있다. 
- 개별 하드웨어는 각자의 방식으로 동작하더라도 이를 사용하는 코드는 공용 인터페이스를 통해서 같은 방식으로 사용할 수 있어야 한다. -> 일종의 **드라이버** 같은 것이 필요한 셈이다. 
- 윈도우나 리눅스의 디바이스 드라이버 레이어는 매우 복잡하지만 그만큼 범용적으로 사용할 수 있다. 하지만 보통 펌웨어에서는 그정도의 범용성을 만족하지 않아도 된다. 
- ☑️  **공용 인터페이스 API**만 정의해놓고 **해당 API를 각자의 하드웨어가 구현**하는 식으로 범용성을 추구한다. **HAL(Hardware Abstraction Layer)**
    - 각 하드웨어의 폴더에 UART를 구현한 C파일을 만든 뒤, 해당 파일로 컴파일한다. 

## 1.2 하드웨어와 함수
- 직접 하드웨어를 건드리는 함수와, 그런 함수를 다시 호출하는 함수가 있다. 
- 각 종류의 함수를 다른 파일에 저장하는게 보통임. 
    - ex) Hal_uart_put_char(uint8_t) : 하드웨어 레지스터에 문자 하나를 입력하는 함수
        - 최적화 하는게 중요! 
        - 레지스터에 접근하는게 좋은지, 혹은 변수에 저장하는게 효율적인지를 잘 판단해야함. 
    - ex) putstr(const char*s) : s포인터의 문자를 출력하며, 각 문자당 Hal_uart_put_char()를 호출함.
- 문자의 입출력이 당연한 것이 아니라 하드웨어를 제어해서 구현하는 기능이라는 것!
- ‼️ **따라서, 우리가 보통 사용하는 함수들은 또 다른 함수(레지스터를 직접 건드리는 근간 함수)를 호출하여 사용되는 것이다!**

+ 추가 : 모든 하드웨어의 사용은 먼저 하드웨어를 초기화하는 것부터 시작된다.

---------------------

# chapter 6 인터럽트

### 인터럽트를 처리하려면??
1. 인터럽트 컨트롤러를 어떻게 사용하는지 알아야 한다.
2. 인터럽트 컨트롤러를 초기화하고 사용하는 코드를 작성해야 한다.
3. 실제 인터럽트를 발생시키는 하드웨어와 인터럽트 컨트롤러를 연결해야 한다.

=> UART하드웨어는 인터럽트를 발생시키므로, 실습에서 UART 하드웨어와 인터럽트 컨트롤러를 연결한다. 
-> UART 하드웨어가 인터럽트 컨트롤러로 인터럽트 신호를 보냄 -> 인터럽트 컨트롤러는 ARM코어로 인터럽트를 보냄. 

## 6.1 인터럽트 컨트롤러
- RealViewPB에는 Generic Interrupt Controller(GIC)라는 이름의 인터럽트 컨트롤러 하드웨어가 존재한다. 
- 가장 먼저 할 일은 GIC의 레지스터 구조체를 만드는 일! (RealViewPB에 포함된 인터럽트 컨트롤러이므로 hal/rvpb 디렉터리에 PL011과 같은 레벨로 레지스터 구조체를 작성해 넣어야 한다. 
    - GIC_DIST(Distributor register) : 인터럽트를 전체 CPU에 분배하는 하드웨어(누가 받을지를 정함) 
        - 초기화 : 모든 인터럽트 비활성화 -> 필요한 것만 enable 
    - GIC_CPU(CPU Interface register) : 특정 CPU가 인터럽트를 실제로 받아서 처리하게 해주는 인터페이스(받은걸 실제로 처리) 
        - 초기화 : 자신이 받을 우선순위나 인터럽트 허용 설정 
    - 이 외에도 GIC는 다양한 레지스터를 가지지만, 실습에서는 이 두가지만 사용함 

- 인터럽트도 UART때 처럼 공용 API를 설계해야함. hal/Interrupt.h
    - 초기화 함수, 인터럽트 활성화 함수, 인터럽트 비활성화 함수, 인터럽트 핸들러 등록 함수, 인터럽트 핸들러를 호출하는 함수
    - ARM 코어의 cspr을 제어해서 코어 수준의 IRQ를 키고 끄는 함수가 필요함. 
        - cspr을 제어하려면 어셈블리어를 사용해야 한다. 
            - 1. .S파일로 어셈블리어 소스 파일을 만들어 완전히 어셈블리어로 작성하는 방법
            - 2. C언어 소스 파일을 만들고 C언어 함수속에서 인라인 어셈블리어를 사용하는 방법
                - 장점 : 스택에 레지스터를 백업 및 복원하는 코디와 리턴 처리하는 코드를 컴파일러가 자동으로 만든다. 어셈블리어로 작성 시, 이 작업을 어셈블리어로 작성해야 함. 

</br>

## 6.2 UART 입력과 인터럽트 연결
- GIC설정을 한 뒤, 인터럽트를 발생시키는 하드웨어를 설정해서 인터럽트 컨트롤러로 인터러브를 보내야 활용할 수 있다. 
- UARTㅇ는 입력, 출력, 에러발생 등등 여러 인터럽트 소스가 있지만, 가장 많이 쓰이면서 직관적인 것은 입력 인터럽트이다.
- UART init함수에 인터럽트를 설정해야함 -> 즉, 하드웨어 초기화 시, 인터럽트 컨트롤러를 초기화한 후 UART를 초기화 해야한다. 
</br>

## 6.3 IRQ 익셉션 벡터 연결
인터럽트가 발생하면 인터럽트 컨트롤러는 이 인터럽트를 접수해서 ARM코어로 바로 전달한다. 
(이 실습에서는 IRQ만 사용함)</br>
- ARM코어는 인터럽트를 받으면 IRQ익셉션을 발생시킨다. 
- 그리고 동작 모드를 IRQ모들 바꾸면서 동시에 익셉션 벡터 테이블의 IRQ익셉션 벡터로 바로 점프한다. -> IRQ익셉션 핸들러가 무조건 실행됨
=> 익셉션 벡터 테이블의 IRQ익셉션 벡터와 인터럽트 컨트롤러의 인터럽트 핸들러를 연결하는 작업이 필요!
</br>


-------------------

# chapter7. 타이머

임베디드 시스템에서는 시간에 의존해서 동작을 제어해야 하는 경우가 많다.</br>
그리고 그 시간을 측정해 주는 것이 **타이머**이다.
</br>

### ⭐ 타이머
- 일반적으로 타이머는 목표 카운트 레지스터와 측정 카운트 레지스터를 조합해서 활용한다. 
- 목표 카운트 레지스터값을 지정하고 측정 카운트 레지스터를 증가 혹은 감소로 설정한다.
    - 증가 : 측정 카운트가 증가할 때에는 0부터 시작해서 목표 카운트값과 같아지면 인터럽트 발생
    - 감소 : 측정 카운트가 목표 카운트값부터 시작해서 0이 되면 인터럽트 발생 
    - 이 동작은 카운터 하드웨어의 구현에 따라 다름 
        - RealViewPB는 SP804라는 타이머 하드웨어를 가지며, 감소하는 형식이다. 
- 일정 시간 간격으로 타이머 인터럽트를 발생시켜서 얼마만큼 시간이 지났는지 알아낼 수 있고, 이걸 활용한게 delay()함수이다.


## 7.1 타이머 하드웨어 초기화
- RealViewPB의 타이머를 사용하므로, rvpb의 폴더에 SP804 하드웨어의 레지스터를 구조체로 추상화하여 추가한다. 
    - Timer_t : SP804 타이머 하드웨어의 전체적인 레지스터 맵 구조 
    - TimerXControl_t : SP804의 timerxcontrol레지스터의 구조
        - 하나의 32비트 레지스터 안에 어떤 비트 필드들이 있는지 정의하며, 구조적으로 timerxcontrol레지스터 하나를 어떻게 다룰지를 명확히 표현함 (uint32t all; : 하나의 32비트 값을 비트 단위로 나눠서 접근하기 위해 사용하는 C언어의 union 문법)
    - TimerXRIST_t, Timer_XMIS_t도 동일한 맥락임 
    - timerxload, timerxvalue에 대한 레지스터 구조체에 대한 정의는 없는데, 이 이유는 timerxload와 timerxvalue는 각각 목표 카운틀 레지스터와 측정 카운트 레지스터이기 때문에 단순히 숫자 값 하나만 넣거나 읽는 레지스터이다. 따라서 비트 단위로 조작할 게 없는 그냥 값 하나이기 때문에 구조체를 따로 정의하지 않음.
    - TimerMode (0) : timerxvalue를 사용하지 않아서 최댓값(16비트 일때는 0xFFFF, 32비트 일때는 0xFFFFFFFF)부터 0까지 카운트가 내려가야 인터럽트가 발생함. => **free running mode**
    - TimerMode (1) : timerxvalue를 사용함 => **periodic mode**


## 7.2 타이머 카운터 오버플로
- TimerSize의 최댓값은 0xFFFFFFFF이다. uint32이므로 10진수로는 42997295이다. 
    - 1ms마다 1씩 증가하므로, 1000씩 올라갈 때마다 1초가 지는 것이다. 
    - 1000으로 나누면 4294967.295이고 이 시간을 날짜로 바꾸면 49.71026이다. 
    - 그러므로 시스템을 50일 정도 계속 켜놓으면 50일이 다 지나기 전에 타이머 카운터는 오버플로를 일으켜서 0이 된다. 
    ** ‼️ 이 시간은 절대 긴 시간이 아니다‼️‼️**
        - 왜냐하면 이 기준은 해당 하드웨어의 동작 목적에 따라 다르기 때문이다. 
        - 어떤 하드웨어는 하루 한 번씩 혹은 50일 이전마다 전원을 껐다가 켜야 하는 것들도 있고, 또 어떤 하드웨어는 몇달 혹은 몇 년이 지나도 전원을 계속 켜놓고 사용해야 하는 것들도 있다. 
        - ** 언제나 최악의 상황에서도 동작하는 소프트웨어를 만들어야 하기 때문에, 타이머 카운터의 오버플로를 고려해야 한다. **


## 7.3 delay() 함수
- 파라미터를 밀리초로 받는다. 
- 임베디드 시스템에서 초 단위는 현 시대의 임베디드 시스템에서는 너무 긴 시간이고, 마이크로초 단위는 변수의 숫자가 너무 커지므로 활용성이 떨어진다. 앞으로 계속 임베디드 시스템이 빨라지고 점점 고도화되면 밀리초가 긴 시간 취급을 받을 날이 올지도 모르지만 아직까지는 밀리초가적당하다. 

- HalTimer.h에 공용 API를 추가한 뒤, rvpb전용 파일로 구현한다. 
- 근간 함수를 반복해서 호출하는 유틸함수이기 때문에, lib디렉터리 안에 stdlib.h/c로 만든다. 
- Main.c에서 하드웨어 초기화 시 타이머 설정도 초기화한다.  

