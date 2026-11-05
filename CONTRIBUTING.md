# Contributing

Everyone taking part in the project follows the [Code of Conduct](.github/CODE_OF_CONDUCT.md).

## Guides

- [Build](docs/build.md): the requirements and the presets that build, test, measure and document the library
- [Conventions](docs/conventions.md): the names, the files, the errors, the Doxygen comments and the rules clang-format does not check
- [Testing](docs/testing.md): the tests and the benchmarks

## Changes

The code must be clean and efficient, with the data structures and the algorithms that fit each problem.
A change to the public API comes with its Doxygen comments and tests, a new module with an example in `examples/example_<module>_<feature>.c`, a bug fix with a regression test, and a new feature with its benchmarks and an update of the roadmap in the README.

## Commits and Pull Requests

Commit messages follow [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/): `<type>[optional scope]: <description>`, in the imperative, starting with a lowercase letter and without a final period.
The subject stays under 72 characters and the lines of the body under 80.
The types are `build`, `cicd`, `chore`, `docs`, `feat`, `fix`, `perf`, `refactor`, `style` and `test`, described in the [instructions of VMNL](https://github.com/VMNL/vmnl/blob/main/docs/INSTRUCTIONS.md#commit-type-usage-guidelines).
Before `1.0.0`, any version may break the API, so breaking changes are not marked with `!`.

Install the hook that checks every message before the commit:

```sh
git config core.hooksPath .githooks
```

A pull request has a title in the format of a commit and a description of a few plain sentences, since it becomes the body of the merge commit.
It closes the issue of its feature with `Closes #N`, and the issue carries the milestone of the version and the labels.
