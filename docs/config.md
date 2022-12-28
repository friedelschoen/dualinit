# DUAL-INIT

## Config Commands

### Controlling

#### `include <path>`

> includes an file

| parameter | description           |
| --------- | --------------------- |
| path      | absolute path to init |

---

#### `end`

> ends a section

### General Configuration

#### `section <name> <root>`

> starts the definition of a section (has to be closed with `end`)

| parameter | description                 |
| --------- | --------------------------- |
| name      | name of the section         |
| root      | path to the root of section |

---

#### `mount`

> starts a mount-section which has to be closed with `end`

---

#### `mount-default <enable>`

> defines if color should be enabled

| parameter                | description                |
| ------------------------ | -------------------------- |
| enable (`true`\|`false`) | if color-output is enabled |

---

#### `mount-master <enable>`

> defines if color should be enabled

| parameter                | description                |
| ------------------------ | -------------------------- |
| enable (`true`\|`false`) | if color-output is enabled |

---

#### `color <enable>`

> defines if color should be enabled

| parameter                | description                |
| ------------------------ | -------------------------- |
| enable (`true`\|`false`) | if color-output is enabled |

---

#### `verbose <enable>`

> defines if color should be enabled

| parameter                | description                  |
| ------------------------ | ---------------------------- |
| enable (`true`\|`false`) | if verbose-output is enabled |

---

#### `timeout <duration>`

> set timeout to duration

| parameter | description        |
| --------- | ------------------ |
| duration  | timeout in seconds |

### Section Configuration

#### `mount`

> starts a mount-section which has to be closed with `end`

---

#### `master`

> defines this section as master

---

#### `default`

> defines this section as default

---

#### `init <path> [args...]`

> defines the init with possible args (defaults to `/sbin/init`)

| parameter | description                          |
| --------- | ------------------------------------ |
| path      | path to init                         |
| args      | arguments you want to pass to `init` |

### Mount Configuration

#### `<type> <source> <target> [options]`

> defines a new mount-point

| parameter | description                     |
| --------- | ------------------------------- |
| type      | partition-type                  |
| source    | source                          |
| target    | target relative to the new root |
| options   | an optional option-list         |
