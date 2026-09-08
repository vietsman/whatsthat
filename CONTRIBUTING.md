# Contributing

Thanks for contributing to **whatsthat**.

## Development

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Milestone workflow

This repository tracks porting milestones from `tulir/whatsmeow`.
Each milestone should be delivered in a focused PR with updated docs and changelog entries.
