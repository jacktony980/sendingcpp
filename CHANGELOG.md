
# Unreleased

- Support streaming uploads. https://gitlab.com/kazv/libkazv/-/merge_requests/1
- Fix ctx.dispatch() returning promises resolving to empty EffectStatus. https://gitlab.com/kazv/libkazv/-/commit/c7796a6ab8325773bf47fe83254a377204d6abcf
- Prevent leaking full file path when uploading to matrix server by using only basename for the request.
