# Pocket Companion

ESP32-S3 기반의 휴대용 AI Companion 프로젝트입니다.

작은 터치 디스플레이에 애니메이션 캐릭터를 표시하고,
사용자의 터치, 움직임, 음성 등에 반응하며
궁극적으로 AI와 자연스럽게 대화할 수 있는 휴대용 디바이스를 만드는 것을 목표로 합니다.

---

## 🎯 Project Goal

Pocket Companion은 스마트폰이나 PC에 의존하지 않고
휴대할 수 있는 작은 AI Companion 디바이스를 만드는 프로젝트입니다.

단순한 음성 비서가 아니라,

- 화면 속 캐릭터와 상호작용
- 터치에 따른 반응
- 움직임에 따른 반응
- 음성으로 대화
- AI를 이용한 자연어 대화
- 대화 및 사용자 정보의 장기적인 기억

등을 하나의 작은 디바이스에 통합하는 것을 목표로 합니다.

---

## ✨ Main Features

### Character

- 화면에 애니메이션 캐릭터 표시
- 다양한 표정
- 대화에 따른 캐릭터 반응
- 눈 깜빡임 및 간단한 애니메이션
- 향후 SD 카드 기반 캐릭터 리소스 관리

### Interaction

- 240 × 320 터치 디스플레이
- 터치 이벤트 처리
- IMU를 이용한 움직임 감지
- 향후 터치와 움직임을 이용한 캐릭터 상호작용

### Voice

- 마이크를 이용한 음성 입력
- 음성 → 텍스트 변환(STT)
- AI 응답 생성
- 텍스트 → 음성 변환(TTS)
- 스피커를 통한 음성 출력

### AI

- 외부 AI API를 이용한 대화
- 자연어 기반 대화
- 대화 맥락 유지
- 장기 기억 기능
- 향후 다양한 AI 모델 및 API 지원

### Connectivity

- Wi-Fi를 통한 인터넷 연결
- 장소에 따라 다른 Wi-Fi 네트워크 연결 지원
- AI API 서버와 통신

### Storage

- SD 카드 사용
- 캐릭터 이미지 및 애니메이션 리소스 저장
- 설정 파일 저장
- 대화 기록 및 장기 기억 데이터 저장

---

# 🛠️ Hardware

현재 사용 중인 주요 하드웨어:

- Waveshare ESP32-S3 Touch LCD
- 240 × 320 IPS Touch Display
- ESP32-S3
- CST816D Capacitive Touch Controller
- IMU Sensor
- Micro SD Card
- Wi-Fi / Bluetooth

### Planned Hardware

- Microphone
- Speaker
- Audio amplifier
- Battery
- Charging circuit

---

# 💻 Development Environment

- PlatformIO
- Visual Studio Code
- Arduino Framework
- LVGL 8.4.0
- Arduino GFX Library
- Waveshare BSP
- C / C++

---

# ✅ Completed

현재까지 구현 및 검증된 기능입니다.

### Hardware & Development Environment

- [x] ESP32-S3 개발 환경 구축
- [x] PlatformIO 프로젝트 구축
- [x] Waveshare ESP32-S3 Touch LCD 연결
- [x] 240 × 320 LCD 출력 확인
- [x] CST816D 터치 입력 확인
- [x] LVGL 환경 구축
- [x] LVGL과 LCD 연동
- [x] LVGL과 터치 입력 연동

### User Interface

- [x] Pocket Companion 기본 UI 구현
- [x] 캐릭터 형태의 기본 UI 구현
- [x] Talk 버튼 구현
- [x] 터치 이벤트 처리
- [x] 2가지 대사 구현
- [x] 2가지 표정 구현
- [x] 버튼 입력에 따른 대사 및 표정 변경

---

# 🚧 Current Development

현재 개발 중인 기능:

- [ ] 실제 캐릭터 이미지 적용
- [ ] 캐릭터 애니메이션
- [ ] 눈 깜빡임
- [ ] 다양한 표정 추가
- [ ] IMU 센서 연동
- [ ] SD 카드 연동
- [ ] 캐릭터 리소스 관리

---

# 📋 Development Roadmap

## Phase 1 — Basic Companion UI

- [x] ESP32-S3 환경 구축
- [x] LCD 및 터치 연결
- [x] LVGL 적용
- [x] 기본 캐릭터 UI
- [x] 터치 상호작용

## Phase 2 — Character

- [ ] 실제 캐릭터 이미지 적용
- [ ] 캐릭터 애니메이션
- [ ] 눈 깜빡임
- [ ] 표정 시스템
- [ ] 대사에 따른 캐릭터 반응
- [ ] IMU를 이용한 움직임 반응

## Phase 3 — Storage

- [ ] SD 카드 초기화
- [ ] 이미지 리소스 저장
- [ ] 애니메이션 리소스 저장
- [ ] 설정 파일 저장
- [ ] 대화 기록 저장

## Phase 4 — Audio

- [ ] 마이크 연결
- [ ] 음성 입력
- [ ] 오디오 전처리
- [ ] STT 적용
- [ ] 스피커 연결
- [ ] TTS 적용

## Phase 5 — Wi-Fi & AI

- [ ] Wi-Fi 연결
- [ ] Wi-Fi 네트워크 변경 지원
- [ ] 인터넷 연결 상태 표시
- [ ] AI API 연동
- [ ] 사용자 음성 → STT → AI → TTS 파이프라인 구현

## Phase 6 — Memory

- [ ] 대화 기록 저장
- [ ] 대화 맥락 유지
- [ ] 사용자 정보 저장
- [ ] 장기 기억 시스템
- [ ] 필요한 기억 검색 기능

## Phase 7 — Portable AI Companion

- [ ] 배터리 전원
- [ ] 충전 기능
- [ ] 절전 기능
- [ ] 부팅 및 종료 관리
- [ ] 휴대 가능한 케이스 제작
- [ ] 최종 하드웨어 통합

---

# 🎯 Final Goal

최종적으로 다음과 같은 기능을 갖춘
소형 휴대용 AI Companion을 구현하는 것을 목표로 합니다.

- 작은 디스플레이에 살아있는 캐릭터가 표시됨
- 사용자의 터치와 움직임에 반응함
- 사용자의 음성을 인식함
- AI와 자연스럽게 대화함
- 대화 내용과 사용자 정보를 기억함
- 캐릭터가 대화 내용과 상황에 따라 표정과 행동을 변경함
- Wi-Fi를 통해 필요한 AI 기능을 사용함
- 배터리로 독립적으로 동작함
- PC 없이 휴대하면서 사용할 수 있음

---

# 📌 Project Status

**Current Version: v0.1.0**

### Current Status

Basic interactive character UI completed.

### Next Milestone

Character animation and IMU-based interaction.