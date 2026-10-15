[🇵🇱 Polski](09_git_i_github.md) | 🇬🇧 English

# 0.9 Git and GitHub: the bigger picture (optional)

This page is **optional**. You don't need anything described here to pass the course — this
course never has you fork a repository, file an issue, or open a pull request. But these concepts
come up constantly in real software work, so it's worth understanding them, so you're not caught
off guard when someone mentions them.

## Git is not GitHub

`Git` is the tool you already use: it records change history locally, on your own disk (`git
clone`, `git status`, `git switch`). It works without the internet and without any particular
company.

`GitHub` is a web service that hosts Git repositories and adds collaboration tools on top: project
pages, issues, pull requests, forks. GitHub isn't the only such service (GitLab and Bitbucket exist
too), but it's the most popular one, and this course uses it as well.

## Fork: your own copy of someone else's repository

A **fork** is your own copy of someone else's repository, under your own GitHub account. You can
do whatever you want with it without affecting the original, because it's a separate copy. Forking
this course's repository would give you your own version at an address like
`github.com/YourName/mechatronic-systems-programming`, independent of
`github.com/michalmaj/mechatronic-systems-programming`.

## Upstream and syncing a fork

The repository a fork came from is called **upstream**. A fork doesn't update itself: if the
original gets new commits, your fork stays on the old version until you sync it. The fork's page
on GitHub has a **Sync fork** button for exactly this. (The same thing can be done from the
terminal, by adding `upstream` as another remote and running `fetch` plus `merge`, but the GitHub
button is simpler.)

## Issue: how to report a problem

An **issue** is a report filed on a GitHub repository: a bug, a question, or a suggested change.
It's not a piece of code — it's text read by whoever maintains the project.

A good issue includes:

1. what you were trying to do,
2. what happened,
3. what should have happened,
4. how to reproduce it (exact steps or a command).

Reports about this course (bugs in the code, tests, or material) go through this repository's
**Issues** tab on GitHub.

## Pull request: how a change gets proposed

A **pull request** (PR) is a request to merge your changes (usually from a fork or a branch) into
another repository. The project's maintainer reviews the change, may ask about it or suggest
fixes, and only then merges it.

This course doesn't use pull requests — your commits stay local, on your own computer. But this is
exactly how the vast majority of open-source projects and companies work, so it's worth knowing
even if you don't need it right now.

## Try it yourself (optional)

If you'd like to see this live rather than just in theory:

1. Create a free account on github.com, if you don't already have one.
2. Go to this repository's page on GitHub and click the **Fork** button in the top-right corner.
3. See your own copy of the repository under your own account.

You don't need to do anything else — this is just to see what a fork looks like in practice. You
don't need to file an issue or open a pull request to pass this module.

## End of Module 0

This really is the end of Module 0. Your environment is ready, you know the basic
change → compile → test cycle, and you understand how Git differs from GitHub. Time for Module 1.
