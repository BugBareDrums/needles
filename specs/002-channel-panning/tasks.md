# Tasks: RGB Channel Panning Controls

**Input**: Design documents from `/specs/002-channel-panning/`
**Prerequisites**: plan.md, spec.md, research.md, data-model.md, contracts/

**Organization**: Tasks are grouped by user story to enable independent implementation and testing of each story.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: Can run in parallel (different files, no dependencies)
- **[Story]**: Which user story this task belongs to (e.g., US1, US2)
- Include exact file paths in descriptions

---

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Project initialization and basic structure for RGB channel panning

- [ ] T001 Verify project dependencies in NeedlesVST/CMakeLists.txt include JUCE audio processing modules
- [ ] T002 [P] Create test infrastructure directories: NeedlesVST/Tests/unit/, NeedlesVST/Tests/integration/, NeedlesVST/Tests/performance/
- [ ] T003 [P] Configure CMake build system to include new test targets in NeedlesVST/CMakeLists.txt

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Core infrastructure that MUST be complete before ANY user story can be implemented

**⚠️ CRITICAL**: No user story work can begin until this phase is complete

- [ ] T004 Create StereoProcessor interface and base implementation in NeedlesVST/Source/StereoProcessor.h
- [ ] T005 [P] Extend ParameterManager interface with RGB pan methods in NeedlesVST/Source/ParameterManager.h
- [ ] T006 [P] Add RGB pan parameter definitions to AudioProcessorValueTreeState in NeedlesVST/Source/PluginProcessor.cpp
- [ ] T007 Implement atomic RGB pan parameter storage in NeedlesVST/Source/ParameterManager.cpp
- [ ] T008 Setup unit test framework for StereoProcessor in NeedlesVST/Tests/unit/StereoProcessorTest.cpp

**Checkpoint**: Foundation ready - user story implementation can now begin in parallel

---

## Phase 3: User Story 1 - Individual RGB Channel Panning (Priority: P1) 🎯 MVP

**Goal**: Enable independent stereo positioning of Red, Green, and Blue color channels with real-time audio feedback

**Independent Test**: Load an image with varied color content → adjust R, G, B pan controls independently → hear distinct stereo positioning for each color channel → verify audio maintains quality and synchronization

### Tests for User Story 1

> **NOTE: Write these tests FIRST, ensure they FAIL before implementation**

- [ ] T009 [P] [US1] Contract test for StereoProcessor pan range validation in NeedlesVST/Tests/unit/StereoProcessorTest.cpp
- [ ] T010 [P] [US1] Contract test for RGB parameter range validation in NeedlesVST/Tests/unit/ParameterRangeTest.cpp
- [ ] T011 [P] [US1] Integration test for RGB channel independence in NeedlesVST/Tests/integration/RGBChannelTest.cpp

### Implementation for User Story 1

- [ ] T012 [P] [US1] Implement constant power panning algorithm in NeedlesVST/Source/StereoProcessor.cpp
- [ ] T013 [P] [US1] Extend RGB struct with per-channel audio extraction in NeedlesVST/Source/AudioSynthesis.cpp
- [ ] T014 [US1] Integrate RGB pan parameters into parameter update loop in NeedlesVST/Source/ParameterManager.cpp
- [ ] T015 [US1] Modify audio processing pipeline to separate RGB channels in NeedlesVST/Source/PluginProcessor.cpp
- [ ] T016 [US1] Apply stereo positioning to individual RGB channels in processBlock method
- [ ] T017 [US1] Mix processed RGB stereo channels to final output buffer
- [ ] T018 [US1] Add RGB pan control sliders to plugin UI in NeedlesVST/Source/PluginEditor.cpp
- [ ] T019 [US1] Connect UI controls to parameter automation system via SliderParameterAttachment

**Checkpoint**: At this point, User Story 1 should be fully functional and testable independently

---

## Phase 4: User Story 2 - Real-time Pan Automation and Control (Priority: P2)

**Goal**: Enable professional automation and real-time modulation of RGB channel panning with DAW integration

**Independent Test**: Map MIDI controller to pan parameters → adjust panning in real-time during audio playbook → verify smooth parameter changes without audio artifacts

### Tests for User Story 2

- [ ] T020 [P] [US2] Contract test for parameter smoothing behavior in NeedlesVST/Tests/unit/ParameterSmoothingTest.cpp
- [ ] T021 [P] [US2] Integration test for DAW automation response in NeedlesVST/Tests/integration/AutomationTest.cpp
- [ ] T022 [P] [US2] Performance test for CPU usage with rapid parameter changes in NeedlesVST/Tests/performance/PanPerformanceTest.cpp

### Implementation for User Story 2

- [ ] T023 [P] [US2] Implement parameter smoothing algorithm in NeedlesVST/Source/ParameterManager.cpp
- [ ] T024 [P] [US2] Add sample-accurate parameter interpolation to audio thread
- [ ] T025 [US2] Integrate parameter smoothing with existing DAW automation in NeedlesVST/Source/PluginProcessor.cpp
- [ ] T026 [US2] Add real-time parameter change validation and clamping
- [ ] T027 [US2] Optimize audio thread performance for rapid parameter updates
- [ ] T028 [US2] Implement artifact-free parameter transitions during extreme automation

**Checkpoint**: At this point, User Stories 1 AND 2 should both work independently

---

## Phase 5: Polish & Cross-Cutting Concerns

**Purpose**: Improvements that affect multiple user stories

- [ ] T029 [P] End-to-end integration test covering complete audio pipeline in NeedlesVST/Tests/integration/StereoOutputTest.cpp
- [ ] T030 [P] Performance validation against constitution requirements (<10% CPU) in NeedlesVST/Tests/performance/PanPerformanceTest.cpp
- [ ] T031 [P] Code cleanup and optimization for real-time audio constraints
- [ ] T032 [P] Update user documentation with RGB pan control usage in quickstart.md validation
- [ ] T033 [P] VST3/AU/AAX parameter validation for cross-platform compatibility
- [ ] T034 Memory usage optimization for stereo processing components
- [ ] T035 Audio quality validation tests for extreme pan positions and automation

---

## Dependencies & Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies - can start immediately
- **Foundational (Phase 2)**: Depends on Setup completion - BLOCKS all user stories
- **User Stories (Phase 3+)**: All depend on Foundational phase completion
  - User stories can then proceed in parallel (if staffed)
  - Or sequentially in priority order (P1 → P2)
- **Polish (Final Phase)**: Depends on all desired user stories being complete

### User Story Dependencies

- **User Story 1 (P1)**: Can start after Foundational (Phase 2) - No dependencies on other stories
- **User Story 2 (P2)**: Can start after Foundational (Phase 2) - Builds on US1 parameter infrastructure but should be independently testable

### Within Each User Story

- Tests MUST be written and FAIL before implementation
- Core StereoProcessor before integration into audio pipeline
- Parameter management before UI integration
- Audio processing pipeline before parameter automation
- Story complete before moving to next priority

### Parallel Opportunities

- All Setup tasks marked [P] can run in parallel
- All Foundational tasks marked [P] can run in parallel (within Phase 2)
- Once Foundational phase completes, both user stories can start in parallel (if team capacity allows)
- All tests for a user story marked [P] can run in parallel
- Different components within a story marked [P] can run in parallel

---

## Parallel Example: User Story 1

```bash
# Launch all tests for User Story 1 together:
Task: "Contract test for StereoProcessor pan range validation in NeedlesVST/Tests/unit/StereoProcessorTest.cpp"
Task: "Contract test for RGB parameter range validation in NeedlesVST/Tests/unit/ParameterRangeTest.cpp"
Task: "Integration test for RGB channel independence in NeedlesVST/Tests/integration/RGBChannelTest.cpp"

# Launch core implementation components together:
Task: "Implement constant power panning algorithm in NeedlesVST/Source/StereoProcessor.cpp"
Task: "Extend RGB struct with per-channel audio extraction in NeedlesVST/Source/AudioSynthesis.cpp"
```

---

## Implementation Strategy

### MVP First (User Story 1 Only)

1. Complete Phase 1: Setup
2. Complete Phase 2: Foundational (CRITICAL - blocks all stories)
3. Complete Phase 3: User Story 1
4. **STOP and VALIDATE**: Test User Story 1 independently using acceptance scenarios
5. Deploy/demo if ready for basic RGB channel panning

### Incremental Delivery

1. Complete Setup + Foundational → Foundation ready
2. Add User Story 1 → Test independently → Deploy/Demo (MVP with basic RGB panning)
3. Add User Story 2 → Test independently → Deploy/Demo (Professional automation features)
4. Each story adds value without breaking previous functionality

### Parallel Team Strategy

With multiple developers:

1. Team completes Setup + Foundational together
2. Once Foundational is done:
   - Developer A: User Story 1 (Core panning functionality)
   - Developer B: User Story 2 (Automation features)
3. Stories complete and integrate independently

---

## Notes

- [P] tasks = different files, no dependencies on incomplete work
- [Story] label maps task to specific user story for traceability
- Each user story should be independently completable and testable
- Verify tests fail before implementing (TDD approach)
- Commit after each task or logical group
- Stop at any checkpoint to validate story independently
- Real-time audio constraints: No locks/blocking in audio thread
- All parameter access must be atomic for thread safety
- Maintain existing CPU performance targets (<10% total plugin usage)
