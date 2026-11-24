# How a shot moves through a VFX studio

## Stages, in order
| Stage | Asset or shot | Takes in | Hands on |
|---|---|---|---|
| Previs | shot | concept, rough story/brief | rough camera and blocking |
| Layout | shot | previs camera and published creature asset | final camera, creature placement, static translate |
| Modelling | asset | approved creature brief/reference | creature geometry and UVs |
| Surfacing | asset | geometry | shaders, maps and material assignments |
| Rigging | asset | approved model and performance needs | controls and deformation rig |
| Animation | shot | layout, rigged asset | translate/rotate/scale keys and performance |
| Creature FX / sim | shot | animated asset | secondary motion, cloth, fur and simulation |
| Lighting and rendering | shot | composed geometry and look choices | lights, lookdev, render settings and output |
| Compositing | shot | rendered passes | depth, colour and effects to produce final pixels |

## Publishing and versioning
A publish is a reviewed, versioned snapshot of work that another department is allowed to consume. Departments normally create a publish rather than editing source files owned by another department, so work can be reproduced and one team’s changes cannot silently rewrite another team’s scene. Asset publishes are reusable across shots; shot work is published as a composed scene for downstream stages.

## Where Creature Performance sits
- Upstream: layout, animation and creature FX receive the published rigged asset plus camera and shot blocking.
- Downstream: lighting and rendering consume the evaluated creature performance and simulation, while compositing consumes the final render passes.
- What a tools developer there builds: validation, USD publishing/composition tools, cache workflows, scene diagnostics and automation that keep rigs, animation, FX and looks consistent.

## Why USD
- One scene description that every department and every package reads.
- Layers let each department add opinions without touching anyone else's file.
- Stronger local opinions in the shot stack beat weaker lower opinions, and references connect reusable assets into shot scenes.
- Variants let one asset expose alternate looks while the shot chooses the intended one.