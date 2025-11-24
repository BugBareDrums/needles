<!--
Sync Impact Report:
- Version: 1.0.0 (initial constitution)
- New principles: All principles are new (I. Code Quality, II. Testing Standards, III. User Experience, IV. Performance Requirements)
- Added sections: Performance Standards, Development Workflow
- Templates updated: ✅ constitution.md (this file), ✅ plan-template.md (constitution check updated)
- Follow-up TODOs: None - all placeholders resolved
-->

# Needles Constitution

## Core Principles

### I. Code Quality (NON-NEGOTIABLE)

Every code change MUST meet quality standards before merge. Code MUST be readable, maintainable, and follow established patterns. All code MUST pass linting, formatting, and static analysis checks without exceptions. Complex logic MUST include explanatory comments. No dead code, console.log statements, or commented-out code in production branches.

**Rationale**: High code quality reduces technical debt, improves maintainability, and ensures consistent developer experience across the team.

### II. Testing Standards (NON-NEGOTIABLE)

Test-Driven Development (TDD) is mandatory: Tests written → Tests fail → Then implement → Tests pass. Every user story MUST have independent integration tests. All contract changes MUST include contract tests. Minimum 80% code coverage required. Tests MUST be deterministic and environment-independent.

**Rationale**: TDD ensures code correctness, prevents regressions, and validates that requirements are testable and well-understood before implementation.

### III. User Experience Excellence

Every feature MUST prioritize user needs over technical convenience. User interfaces MUST be intuitive, responsive, and accessible. Error messages MUST be user-friendly with clear action paths. Performance MUST feel instantaneous for common operations (<200ms response time). Loading states and feedback MUST be provided for operations >100ms.

**Rationale**: User experience drives product success and adoption. Technical excellence without user focus delivers no business value.

### IV. Performance Requirements

All features MUST meet performance benchmarks: Web responses <200ms p95, API throughput >1000 req/sec, mobile app startup <2 seconds, database queries <50ms p95. Performance testing MUST be included in CI/CD pipeline. Performance regressions block deployment.

**Rationale**: Performance is a feature, not an optimization. Poor performance directly impacts user satisfaction and business metrics.

## Performance Standards

**Response Time Targets**:

- Interactive operations: <100ms
- Page loads: <200ms p95
- API responses: <200ms p95
- Database queries: <50ms p95
- Mobile app startup: <2 seconds

**Throughput Targets**:

- API endpoints: >1000 requests/second
- Background processing: >10,000 items/hour
- Real-time features: <50ms latency

**Resource Constraints**:

- Memory usage: <500MB per service
- CPU utilization: <70% sustained
- Storage growth: <10% monthly without data growth

## Development Workflow

**Code Review Process**:

- All changes require peer review before merge
- Reviews MUST verify constitution compliance
- Constitution violations require explicit justification and approval
- Security and performance implications MUST be assessed

**Quality Gates**:

- All automated tests MUST pass
- Code coverage MUST meet minimum thresholds
- Static analysis MUST show no high-severity issues
- Performance benchmarks MUST be maintained

**Documentation Requirements**:

- User-facing features require quickstart documentation
- Complex technical decisions require architectural documentation
- Breaking changes require migration guides

## Governance

This constitution supersedes all other development practices and standards. All pull requests and code reviews MUST verify compliance with these principles. Any violations MUST be explicitly justified with business rationale and approved by technical leadership.

Constitution amendments require:

1. Documented proposal with rationale
2. Impact assessment on existing code
3. Migration plan for non-compliant code
4. Team consensus and formal approval

Complexity that violates simplicity principles MUST be justified with:

- Clear business necessity
- Evidence that simpler alternatives are insufficient
- Maintenance plan for the added complexity

**Version**: 1.0.0 | **Ratified**: 2024-11-24 | **Last Amended**: 2024-11-24
