<!--
Author: Michael Garcia, M&E Design
License: MIT
Date: 2026-04-17
Contact: michael@mandedesign.studio
Website: https://mandedesign.studio
-->

# Coding Standards

## Core Expectations

- Code must be readable without narration from the author.
- Behavior must be deterministic unless non-determinism is a stated requirement.
- Abstractions must reduce complexity, not relocate it.
- Production code must prefer explicit data flow over implicit side effects.

## Structure Rules

- Keep functions and methods focused on one coherent behavior.
- Prefer composition over deep inheritance or implicit framework magic.
- Isolate I/O, time, randomness, and external systems behind testable seams.
- Make units, ranges, and failure assumptions visible near the code that uses them.

## Reliability Rules

- Validate inputs at trust boundaries.
- Fail fast on invalid internal state.
- Use defensive checks where incorrect behavior would be silent or hazardous.
- Avoid best-effort behavior in critical paths unless the degraded mode is defined and acceptable.

## Determinism Rules

- Control ordering where order affects correctness.
- Avoid hidden global state.
- Use explicit seeds or recorded randomness sources when stochastic behavior is required.
- Ensure repeated execution under the same inputs yields the same result unless documented otherwise.

## Review Criteria

- Is the intent obvious?
- Are boundaries clear?
- Are edge cases handled deliberately?
- Is the code easier to change than the prior state?
