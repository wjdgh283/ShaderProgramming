# ShaderProgramming

Kim Pope님의 책 [셰이더프로그래밍 입문] 책을 보며 공부했던 내용입니다.

압축파일 안에는 챕터별로 폴더가 있고, 각 폴더에는 솔루션 파일과 렌더몽키 실행파일이 있습니다.

렌더몽키 실행파일로 정점셰이더와 픽셀셰이더 코드를 보실 수 있습니다.

솔루션파일을 빌드하면 DiretX프레임워크 상에서 셰이더결과를 보실 수 있습니다.

아래 그림은 셰이더의 개념과 각 챕터별 결과화면입니다.

### Chaper1.

**1.셰이더란?**

 ■ 어휘적 접근 : 픽셀의 농담, 색조, 명암을 결정하는 주체

 ■ 구조적 접근 : 정점셰이더 : 투시 / 픽셀셰이더 : 명암![img](https://lh4.googleusercontent.com/f46fnpmkKQiN80jVeIv-p__iMWcuoiqMfIg2YyhbDZQuj8aHjIT87Ltu4xUAQzwyx72vKJVrHpLofVwQ_KBSZ8ZpH49ZHANlK6M47jXU1sAEgndV-_73Hll3Q1K2YK0cyyCcGf6H)

<br><br><br>

### **Chaper2.**

기본적인 빨강색 셰이더입니다

![img](https://github.com/wjdgh283/ShaderProgramming/blob/master/img%20rcs/2.JPG?raw=true)

<br><br><br>

### **Chaper3.**

물체표면을 단색으로 출력하지 않고, 텍스처를 입히는 텍스처매핑입니다

![img](https://github.com/wjdgh283/ShaderProgramming/blob/master/img%20rcs/3.JPG?raw=true)

<br><br><br>

### **Chaper4.**

광원을 추가하여 정반사광과 난반사광을 표현한 셰이더입니다.

![img](https://github.com/wjdgh283/ShaderProgramming/blob/master/img%20rcs/4.JPG?raw=true)

<br><br><br>

### **Chaper5.**

난반사광과 정반사광에 텍스처를 적용한 디퓨즈/스페큘러 매핑입니다. 

![img](https://github.com/wjdgh283/ShaderProgramming/blob/master/img%20rcs/5.JPG?raw=true)

<br><br><br>

### **Chaper6.**

만화같은 명암을 입히는 툰셰이더입니다. 명암처리를 부드럽게 하는대신 2~3단계로 딱딱 끊어서 표현하였습니다.

![img](https://github.com/wjdgh283/ShaderProgramming/blob/master/img%20rcs/6.JPG?raw=true)

<br><br><br>

### **Chaper7.**

폴리곤 수를 늘리지 않고도 표면의 울퉁불퉁한 효과를 추가하는 법선매핑을 적용한 결과입니다. 표면 위에서 법선이 어떻게 정의되어 있는냐에 따라서 조명효과가 결정됩니다.

![img](https://github.com/wjdgh283/ShaderProgramming/blob/master/img%20rcs/7.JPG?raw=true)

<br><br><br>

### **Chaper8.**

입방체 텍스처를 추가하여 환경매핑을 한 결과입니다. 즉 거울처럼 주위환경이 표면에 반사되는 걸 재현한 것입니다. 주위환경을 미리 텍스처 안에 저장해 놓은 뒤, 실행도중 실시간으로 그 텍스처를 입히는 원리입니다. 환경매핑은 주위에 있는 물체가 반사한 빛이 거울 같은 표면에 정반사되는 것입니다.

![img](https://github.com/wjdgh283/ShaderProgramming/blob/master/img%20rcs/8.JPG?raw=true)

<br><br><br>

### **Chaper9.**

UB애니메이션으로 울렁이는 효과를 재현해낸 것입니다. 시간이 흐름에 따라 U좌표에 더해주는 값을 서서히 증가시키거나 빼주면 텍스처를 좌우로 움직일 수 있게 됩니다. 추가로 정점의 위치가 위아래로 출렁이게 하기 위해 정점위치에 Y값을 적당히 증감해주었습니다.

![img](https://github.com/wjdgh283/ShaderProgramming/blob/master/img%20rcs/9-1.gif?raw=true)

<br><br><br>

### **Chaper10.**

그림자 기법을 평정한 그림자매핑입니다. 다음과 같은 순서로 셰이더를 만듭니다.

1. 그림자생성단계
   - 렌더링 결과(빛을 가로막는 첫 번째 물체의 깊이)를 저장할 렌더타겟을 정해 준다.
   - 카메라를 광원의 위치에 두고 물체들을 그린다.
   - 픽셀셰이더에서 빛으로부터 현재 픽셀까지의  깊이를 반환한다.
2. 그림자 적용단게
   - 렌더링 결과(일반 장면 렌더링)을 화면(백버퍼)에 저장한다.
   - 카메라를 눈의 위치에 두고 물체들을 그린다.
   - 빛으로부터 현재 픽셀까지의  깊이를 그림자맵에 담겨있는 결과와 비교한다. 현재 깊이가 그림자맵의 깊이보다 크면 그림자를 씌운다.

![img](https://github.com/wjdgh283/ShaderProgramming/blob/master/img%20rcs/10.JPG?raw=true)

<br><br><br>

### **Chaper11.**

세피아 / 흑백사진 효과입니다. 장면을 그린 뒤, 그 결과를 2D이미지로 가져다가 한번에 고쳐버리는 원리입니다.

![img](https://github.com/wjdgh283/ShaderProgramming/blob/master/img%20rcs/11-1.JPG?raw=true)

![img](https://github.com/wjdgh283/ShaderProgramming/blob/master/img%20rcs/11-2.JPG?raw=true)

<br><br><br>

### **Chaper12.**

외곽선 찾기 / 양각효과 입니다. 배경과 물체의 색이나 명암이 다르면 물체의 외곽선을 볼 수있게 한 결과입니다. 현재 픽셀을 중심으로 해서 그 주위에 있는 픽셀마다 가중치를 곱한 뒤, 그 결과를 모두 더한 값으로 현재 픽셀의 값을 변경하는 컨벌루션 연산을 이용합니다.

![img](https://github.com/wjdgh283/ShaderProgramming/blob/master/img%20rcs/12-1.JPG?raw=true)

![img](https://github.com/wjdgh283/ShaderProgramming/blob/master/img%20rcs/12-2.JPG?raw=true)