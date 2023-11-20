

# Software Development Process (SDP)

## Principles

- We use Discord to communicate asynchronously and answer as soon as possible – within 24 hours at the latest.
- We use a GitHub organization to house our repositories and projects.
- We use GitHub projects to organize a Kanban board to-do list and continuously work on it such that we always have something to work on.
- We move tasks around on the Kanban board as we pick up tasks and turn them into GitHub Issues.
- We communicate when we pick up tasks to keep the group up to date.
- All changes need to be developed in a separate git branch and pushed to a pull request.
- The pull request must be reviewed and approved by at least 1 team member before it can be merged.
- The pull request must pass the linting workflow set up on GitHub Actions.
- Code must be commented, easy to read, and modularized.

## Process

1. Create GitHub Project tasks from planning and research, add to the backlog (Backlog, In Progress, In Review, Done).
2. Discuss, as a group, task assignees.
3. As a task gets picked up, create a GitHub Issue and update the task board.
4. Use GitHub Issues to facilitate and document discussion on tasks.
5. Demo/Review with Project Partner (1/week).
6. Group meetings on a need-be basis, with constant communication on Discord.

## Rules (Reformatted as Principles)

| Principle | Description |
|-----------|-------------|
| Communication | Discord for asynchronous communication, responses within 24 hours |
| Repository Management | GitHub organization for repositories and projects |
| Task Management | Kanban board on GitHub for continuous work and task tracking |
| Issue Tracking | Tasks are turned into GitHub Issues, communication upon task pickup |
| Development Workflow | Changes developed in separate branches, pushed to pull requests |
| Review Process | Pull requests reviewed by at least one team member |
| Code Quality | Pull requests must pass linting; code must be commented, readable, modularized |

## Tooling

| Tool Category | Tools Used |
|---------------|------------|
| Version Control | GitHub |
| Project Management | GitHub Issues and Projects |
| Documentation | Google Docs, README files |
| Test Framework | Physical hardware (robot) |
| Linting/Formatting | Platformio for Arduino |
| CI/CD | GitHub Actions |
| IDE | Visual Studio Code |
| Graphic Design | Figma, Pen and Paper, Google Drawing |
| Other | Fusion 360 (CAD) |

## Definition of Done (DoD)

- Ensure that the RoboRacer meets all specified requirements for scale options, line following, speed profiles, and user interface features.
- Code and design changes related to the RoboRacer must be reviewed and approved by at least one other team member.
- Changes must be consistently implemented across the RoboRacer's hardware and software components, including its control systems, motion tracking, and user interface.
- Ensure new updates do not negatively impact the RoboRacer's existing functionalities like speed control, line tracking, and battery efficiency.
- Minimize and properly handle major changes in the system to ensure seamless integration.
- Ensure updates do not impact RoboRacer's key performance metrics such as speed accuracy and battery life.
- Adjust RoboRacer based on team reviews and user feedback to optimize its performance and user experience.

## Release Cycle

- Automatically deploy changes to the prerelease environment every time there is a merge operation on the master branch.
- Deploy updates to the production environment with each official release.
- Use semantic versioning MAJOR.minor.patch
- The minor version number is incremented whenever new features are added to the project (e.g., new training modes or user interface updates).
- The patch number is increased when bugfixes are made to the project (such as correcting speed accuracy or battery performance issues).
- The major version number is increased if the project has a disruptive API change (e.g., changes to data interaction formats or control protocols).

## Environments

| Environment | Infrastructure | Deployment | Purpose | Monitoring |
|-------------|----------------|------------|---------|------------|
| Production | Robot (Physical device) | Release | Robot functions | Human |
| Staging (Test) | GitHub through CI/CD | PR | Testing new features, integration tests | Super-Linter |
| Development | Local (macOS and Windows) | Commit | Development, unit tests | N/A |



