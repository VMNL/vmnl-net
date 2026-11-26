# Contributing

Everyone taking part in the project follows the [Code of Conduct](CODE_OF_CONDUCT.md).

## Guides

- [Build](docs/build.md): the requirements and the presets that build, test, measure and document the library
- [Conventions](docs/conventions.md): the names, the files, the errors, the Doxygen comments and the rules clang-format and clang-tidy do not check
- [Quality](docs/quality.md): the checks of the CI, the changes they stopped, the clang-tidy rules and the technical debt
- [Testing](docs/testing.md): the tests and the benchmarks

## Layout

- `include/vmnl/net/`: the public headers, one per module
- `src/<module>/`: the sources and the private header of the module
- `tests/<module>/`: the tests of each module
- `bench/`: the benchmarks of each module
- `examples/`: the examples of the modules
- `docs/`: the guides, the references, the research log, the architecture and the decision records
- `cmake/`: the CMake modules and the pkg-config template
- `tools/`: the scripts that compare the benchmarks and print the docs to PDF
- `.github/`: the workflows and the Dependabot configuration
- `.githooks/`: the hook that checks the commit messages

## Changes

The code must be clean and efficient, with the data structures and the algorithms that fit each problem.
A change to the public API comes with its Doxygen comments and tests, a new module with an example in `examples/example_<module>_<feature>.c`, a bug fix with a regression test, and a new feature with its benchmarks and an update of the roadmap in the README.

## AI

AI tools are welcome, as long as the author reviews every line before pushing it and the code shows none of the usual patterns of generated code.
Using AI to make commits without taking the time to actively understand the project is pointless.
One push has one contributor, its author, with no AI co-author nor trailer such as `Co-Authored-By`.

## Commits and Pull Requests

Commit messages follow [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/): `<type>[optional scope]: <description>`, in the imperative, starting with a lowercase letter and without a final period.
The subject stays under 72 characters and the lines of the body under 80.
The types are `build`, `cicd`, `chore`, `docs`, `feat`, `fix`, `perf`, `refactor`, `style` and `test`, described in the [instructions of VMNL](https://github.com/VMNL/vmnl-graphics/blob/main/docs/INSTRUCTIONS.md#commit-type-usage-guidelines).
Before `1.0.0`, any version may break the API, so breaking changes are not marked with `!`.

Install the hook that checks every message before the commit:

```sh
git config core.hooksPath .githooks
```

A pull request has a title in the format of a commit and a description of a few plain sentences, since it becomes the body of the merge commit.
It closes the issue of its feature with `Closes #N`, and the issue carries the milestone of the version and the labels.

## Issues

An issue has a title in the format of a commit, the labels of its type, its areas and its impacts, and the milestone of its version once it is planned.
Its description depends on who takes it:

- a feature or a change planned in a version describes its result in one sentence that starts with what it adds, like `Error codes for the failures of the network layer, named after errno: would block, interrupted, …`
- a task left to contributors, like a good first issue, explains its context in a paragraph, then lists the steps under `What to do:`, ends with the command that checks the result, and links the guides it relies on

