# Tasks: Needles VST Plugin

**Input**: Design documents from `/specs/001-needles/`
**Prerequisites**: plan.md (required), spec.md (required for user stories), research.md, data-model.md, contracts/

**Tests**: Tests are mandatory following constitution TDD requirements.

**Organization**: Tasks are grouped by user story to enable independent implementation and testing of each story.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: Can run in parallel (different files, no dependencies)
- **[Story]**: Which user story this task belongs to (e.g., US1, US2, US3)
- Include exact file paths in descriptions

## Path Conventions

Based on JUCE plugin structure from plan.md:

- **Project root**: `NeedlesVST/`
- **Source code**: `Source/`
- **Tests**: `Tests/Unit/`, `Tests/Integration/`
- **Resources**: `Resources/`

---

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: JUCE project initialization and basic VST plugin structure

- [ ] T001 Create JUCE project file NeedlesVST.jucer with VST3 plugin configuration
- [ ] T002 [P] Initialize project directory structure per plan.md layout
- [ ] T003 [P] Configure build systems (Visual Studio, Xcode, Linux Makefile) via Projucer
- [ ] T004 [P] Setup Catch2 testing framework in Tests/ directory
- [ ] T005 [P] Configure clang-format and cppcheck for code quality standards
- [ ] T006 Create base PluginProcessor stub in Source/PluginProcessor.cpp/h
- [ ] T007 Create base PluginEditor stub in Source/PluginEditor.cpp/h

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Core infrastructure that MUST be complete before ANY user story can be implemented

**⚠️ CRITICAL**: No user story work can begin until this phase is complete

- [ ] T008 [P] Implement RGB data structure in Source/AudioSynthesis.h
- [ ] T009 [P] Implement Position and Dimensions structures in Source/ImageScanner.h
- [ ] T010 [P] Create IImageLoader interface in Source/ImageLoader.h
- [ ] T011 [P] Create IImageScanner interface in Source/ImageScanner.h
- [ ] T012 [P] Create IAudioSynthesis interface in Source/AudioSynthesis.h
- [ ] T013 [P] Create IParameterManager interface in Source/ParameterManager.h
- [ ] T014 [P] Create IPluginState interface in Source/PluginState.h
- [ ] T015 Setup JUCE AudioProcessorValueTreeState in Source/PluginProcessor.cpp
- [ ] T016 Configure basic parameter definitions (scan speed, area size) in Source/PluginProcessor.cpp
- [ ] T017 Implement basic audio processing loop skeleton in Source/PluginProcessor.cpp

**Checkpoint**: Foundation ready - user story implementation can now begin in parallel

---

## Phase 3: User Story 1 - Instant Audio Generation from Image Loading (Priority: P1) 🎯 MVP

**Goal**: User loads image file and immediately hears continuous audio with infinite looping

**Independent Test**: Load any image in DAW → audio starts immediately → loops continuously

### Tests for User Story 1 ⚠️

> **NOTE: Write these tests FIRST, ensure they FAIL before implementation**

- [ ] T018 [P] [US1] Contract test for IImageLoader.loadImage() in Tests/Unit/ImageLoaderTest.cpp
- [ ] T019 [P] [US1] Contract test for IImageLoader.getPixel() in Tests/Unit/ImageLoaderTest.cpp
- [ ] T020 [P] [US1] Contract test for IImageLoader.getAreaAverage() in Tests/Unit/ImageLoaderTest.cpp
- [ ] T021 [P] [US1] Contract test for IAudioSynthesis.rgbToAudio() in Tests/Unit/AudioSynthesisTest.cpp
- [ ] T022 [P] [US1] Contract test for IImageScanner basic path tracing in Tests/Unit/ImageScannerTest.cpp
- [ ] T023 [P] [US1] Integration test for complete image-to-audio workflow in Tests/Integration/NeedlesWorkflowTest.cpp

### Implementation for User Story 1

- [ ] T024 [P] [US1] Implement ImageLoader class with JUCE::Image integration in Source/ImageLoader.cpp
- [ ] T025 [P] [US1] Implement basic RGB pixel extraction with boundary checking in Source/ImageLoader.cpp
- [ ] T026 [P] [US1] Implement area averaging algorithm with configurable radius in Source/ImageLoader.cpp
- [ ] T027 [US1] Implement ImageScanner class with alternating scanline pattern in Source/ImageScanner.cpp (depends on T024-T026)
- [ ] T028 [US1] Implement infinite looping logic with position reset in Source/ImageScanner.cpp
- [ ] T029 [US1] Implement AudioSynthesis class with RGB-to-audio conversion in Source/AudioSynthesis.cpp
- [ ] T030 [US1] Implement basic file loading dialog integration in Source/PluginEditor.cpp
- [ ] T031 [US1] Implement image display component in plugin UI in Source/PluginEditor.cpp
- [ ] T032 [US1] Integrate image loading with automatic audio start in Source/PluginProcessor.cpp
- [ ] T033 [US1] Implement real-time audio generation in processBlock() in Source/PluginProcessor.cpp
- [ ] T034 [US1] Add error handling for unsupported/corrupted image files in Source/PluginProcessor.cpp

**Checkpoint**: At this point, User Story 1 should be fully functional and testable independently

---

## Phase 4: User Story 2 - Real-time Parameter Control (Priority: P2)

**Goal**: User adjusts scan speed and area size parameters with immediate audio feedback

**Independent Test**: Load image → adjust parameters → hear real-time audio changes

### Tests for User Story 2 ⚠️

- [ ] T035 [P] [US2] Contract test for IParameterManager parameter value access in Tests/Unit/ParameterManagerTest.cpp
- [ ] T036 [P] [US2] Contract test for parameter smoothing integration in Tests/Unit/ParameterManagerTest.cpp
- [ ] T037 [P] [US2] Integration test for real-time parameter changes in Tests/Integration/ParameterUpdateTest.cpp

### Implementation for User Story 2

- [ ] T038 [P] [US2] Implement ParameterManager class with thread-safe access in Source/ParameterManager.cpp
- [ ] T039 [P] [US2] Add scan speed parameter with smoothing in Source/ParameterManager.cpp
- [ ] T040 [P] [US2] Add area size parameter with smoothing in Source/ParameterManager.cpp
- [ ] T041 [US2] Integrate parameter smoothing with audio processing thread in Source/PluginProcessor.cpp
- [ ] T042 [US2] Implement real-time scan speed control in Source/ImageScanner.cpp
- [ ] T043 [US2] Implement real-time area size control in Source/AudioSynthesis.cpp
- [ ] T044 [US2] Add parameter control UI elements (sliders) in Source/PluginEditor.cpp
- [ ] T045 [US2] Connect UI parameter changes to AudioProcessorValueTreeState in Source/PluginEditor.cpp
- [ ] T046 [US2] Add visual feedback for current scan position on image in Source/PluginEditor.cpp

**Checkpoint**: At this point, User Stories 1 AND 2 should both work independently

---

## Phase 5: User Story 3 - Advanced Synthesis Control (Priority: P3)

**Goal**: User explores different RGB conversion formulas and scanning patterns

**Independent Test**: Select different conversion methods → hear distinct audio characteristics

### Tests for User Story 3 ⚠️

- [ ] T047 [P] [US3] Contract test for ConversionFormula enumeration in Tests/Unit/AudioSynthesisTest.cpp
- [ ] T048 [P] [US3] Contract test for ScanPattern enumeration in Tests/Unit/ImageScannerTest.cpp
- [ ] T049 [P] [US3] Integration test for formula switching during audio playback in Tests/Integration/AdvancedSynthesisTest.cpp

### Implementation for User Story 3

- [ ] T050 [P] [US3] Implement ConversionFormula enum with multiple RGB algorithms in Source/AudioSynthesis.h
- [ ] T051 [P] [US3] Implement ScanPattern enum with different path algorithms in Source/ImageScanner.h
- [ ] T052 [US3] Implement weighted RGB conversion (luminance formula) in Source/AudioSynthesis.cpp
- [ ] T053 [US3] Implement individual channel extraction (R, G, B only) in Source/AudioSynthesis.cpp
- [ ] T054 [US3] Implement min/max channel selection algorithms in Source/AudioSynthesis.cpp
- [ ] T055 [US3] Implement vertical scanning pattern in Source/ImageScanner.cpp
- [ ] T056 [US3] Implement diagonal scanning pattern in Source/ImageScanner.cpp
- [ ] T057 [US3] Add conversion formula selector UI control in Source/PluginEditor.cpp
- [ ] T058 [US3] Add scan pattern selector UI control in Source/PluginEditor.cpp
- [ ] T059 [US3] Integrate formula/pattern selection with real-time audio in Source/PluginProcessor.cpp

**Checkpoint**: All user stories should now be independently functional

---

## Phase 6: Polish & Cross-Cutting Concerns

**Purpose**: Improvements that affect multiple user stories

- [ ] T060 [P] Implement PluginState class for DAW project persistence in Source/PluginState.cpp
- [ ] T061 [P] Add comprehensive input validation for all parameters in Source/ParameterManager.cpp
- [ ] T062 [P] Implement memory optimization for large images in Source/ImageLoader.cpp
- [ ] T063 [P] Add performance monitoring and CPU usage optimization in Source/PluginProcessor.cpp
- [ ] T064 [P] Create comprehensive unit test coverage (aim for 80%+ per constitution)
- [ ] T065 [P] Add error logging and diagnostic information in Source/PluginProcessor.cpp
- [ ] T066 [P] Implement plugin metadata and version information in Source/PluginProcessor.cpp
- [ ] T067 [P] Create user documentation and quickstart validation per quickstart.md
- [ ] T068 Code cleanup and refactoring for maintainability across all files
- [ ] T069 Performance optimization and memory leak detection across all components
- [ ] T070 Cross-platform compatibility testing (Windows, macOS, Linux)

---

## Dependencies & Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies - can start immediately
- **Foundational (Phase 2)**: Depends on Setup completion - BLOCKS all user stories
- **User Stories (Phase 3-5)**: All depend on Foundational phase completion
  - User stories can then proceed in parallel (if staffed)
  - Or sequentially in priority order (P1 → P2 → P3)
- **Polish (Phase 6)**: Depends on all desired user stories being complete

### User Story Dependencies

- **User Story 1 (P1)**: Can start after Foundational (Phase 2) - No dependencies on other stories
- **User Story 2 (P2)**: Can start after Foundational (Phase 2) - Integrates with US1 but independently testable
- **User Story 3 (P3)**: Can start after Foundational (Phase 2) - Builds on US1/US2 but independently testable

### Within Each User Story

- Tests MUST be written and FAIL before implementation
- Interface implementations before concrete classes
- Core algorithms before UI integration
- Error handling after basic functionality
- Story complete before moving to next priority

### Parallel Opportunities

- All Setup tasks marked [P] can run in parallel
- All Foundational interface definitions can run in parallel (within Phase 2)
- Once Foundational phase completes, all user stories can start in parallel (if team capacity allows)
- All tests for a user story marked [P] can run in parallel
- Different implementation components within a story marked [P] can run in parallel
- Different user stories can be worked on in parallel by different team members

---

## Parallel Example: User Story 1

````bash
# Launch all tests for User Story 1 together:
Task: "Contract test for IImageLoader.loadImage() in Tests/Unit/ImageLoaderTest.cpp"
Task: "Contract test for IImageLoader.getPixel() in Tests/Unit/ImageLoaderTest.cpp"
Task: "Contract test for IAudioSynthesis.rgbToAudio() in Tests/Unit/AudioSynthesisTest.cpp"

# Launch all parallel implementation tasks for User Story 1:
Task: "Implement ImageLoader class with JUCE::Image integration in Source/ImageLoader.cpp"
Task: "Implement basic RGB pixel extraction in Source/ImageLoader.cpp"
Task: "Implement area averaging algorithm in Source/ImageLoader.cpp"
```---

## Implementation Strategy

### MVP First (User Story 1 Only)

1. Complete Phase 1: Setup
2. Complete Phase 2: Foundational (CRITICAL - blocks all stories)
3. Complete Phase 3: User Story 1
4. **STOP and VALIDATE**: Test User Story 1 independently in multiple DAWs
5. Deploy/demo if ready

### Incremental Delivery

1. Complete Setup + Foundational → Foundation ready
2. Add User Story 1 → Test independently → Deploy/Demo (MVP!)
3. Add User Story 2 → Test independently → Deploy/Demo
4. Add User Story 3 → Test independently → Deploy/Demo
5. Each story adds value without breaking previous stories

### Parallel Team Strategy

With multiple developers:

1. Team completes Setup + Foundational together
2. Once Foundational is done:
   - Developer A: User Story 1 (P1 - Core functionality)
   - Developer B: User Story 2 (P2 - Parameter control)
   - Developer C: User Story 3 (P3 - Advanced features)
3. Stories complete and integrate independently

---

## Notes

- [P] tasks = different files, no dependencies
- [Story] label maps task to specific user story for traceability
- Each user story should be independently completable and testable
- TDD approach: Write tests first, ensure they fail before implementing
- Commit after each task or logical group
- Stop at any checkpoint to validate story independently
- Follow JUCE best practices for audio thread safety
- Maintain <10% CPU usage and <50ms parameter response throughout development
- Avoid: vague tasks, same file conflicts, cross-story dependencies that break independence
````
