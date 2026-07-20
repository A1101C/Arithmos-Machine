# Explicanda

> **This branch `archive/v0.1-legacy` is a historical archive.**  
> It represents the final state of the initial architecture before the a major rewrite.

---

### Retrospective & Architectural Limits

This version reached scaling limits due to relying on procedural evaluation. Expanding into calculus and supporting complex mathematical functions necessitated a fundamental shift in build ideology and core architecture.

#### Key Achievements of this version:
* Built a foundational calculation loop capable of parsing/evaluating mathematical string inputs.
* Implemented graphing logic to generate coordinate sets $(x, y)$ for functions of $x$.
* Built full DevOps pipeline: feature-branch workflow, pull request checks, and automated Docker container builds/updates via CI/CD.

#### Lessons Carried Into the future:
Building this project provided hands on experience across the entire software development lifecycle. From raw mathematical logic to engineering and deployment across the entire web software stack, backend, front end, and api middleware. The architectural limits identified here served as the direct foundation for the system design and development roadmap moving forward.