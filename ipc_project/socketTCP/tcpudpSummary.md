### 1. TCP (Transmission Control Protocol)
- connection-oriented service => 연결이 성공해야 통신이 가능함
	- 3-way handshaking(연결), 4-way handshaking(해제)
- byte stream service (데이터의 경계를 구분하지 않음 => 즉, 패킷 단위로 터를 관리하지 않고, 바이트 하나하나를 stream처럼 처리한다.) 
	-> 메시지의 경계를 보장하지 않으므로, 애플리케이션에서 이를 해결해야 함. (고정 길이 메시지 사용, 특정 구분자 사용, 길이 정보 포함)
-  데이터의 전송 순서를 보장한다. 데이터의 순서 유지를 위해 각 바이트마다 번호가 부여됨. 
- 신뢰성이 있는 데이터를 전송한다. (Sequence Number, Ack Number)
- 데이터 흐름 제어 및 혼잡 제어
- 1:1 통신만 가능함

<br>

### 2. UDP (User Datagram Protocol)
- 비연결형 서비스 -> 연결 과정이 없다.
- 상대적으로 TCP보다 전송 속도가 빠름
- 데이터의 신뢰성이 없다.
- 데이터의 경계를 구분한다.
- N:N 통신 가능
