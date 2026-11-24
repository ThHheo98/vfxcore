from pathlib import Path

from pxr import Gf, Sdf, Usd, UsdGeom


ROOT = Path(__file__).resolve().parents[2]
USD_DIR = ROOT / "build" / "usd"


def build_asset(path: Path) -> None:
    stage = Usd.Stage.CreateNew(str(path))
    stage.SetMetadata("metersPerUnit", 1.0)
    stage.SetMetadata("upAxis", UsdGeom.Tokens.y)

    hero = UsdGeom.Sphere.Define(stage, "/World/Hero").GetPrim()
    stage.SetDefaultPrim(hero)
    hero.CreateAttribute("creatureKind", Sdf.ValueTypeNames.String).Set("hydra")

    looks = hero.GetVariantSet("look")
    looks.AddVariant("clean")
    looks.AddVariant("muddy")

    looks.SetVariantSelection("clean")
    with looks.GetVariantEditContext():
        UsdGeom.Sphere.Get(stage, "/World/Hero").GetDisplayColorAttr().Set([(0.8, 0.8, 0.75)])

    looks.SetVariantSelection("muddy")
    with looks.GetVariantEditContext():
        UsdGeom.Sphere.Get(stage, "/World/Hero").GetDisplayColorAttr().Set([(0.28, 0.18, 0.08)])

    looks.SetVariantSelection("clean")
    stage.GetRootLayer().Save()


def build_layout(path: Path) -> None:
    stage = Usd.Stage.CreateNew(str(path))
    hero = UsdGeom.Xform.Define(stage, "/World/Hero")
    hero.GetPrim().GetReferences().AddReference("./asset.usda", "/World/Hero")
    hero.AddTranslateOp().Set(Gf.Vec3d(5.0, 0.0, 0.0))
    stage.GetRootLayer().Save()


def build_anim(path: Path) -> None:
    stage = Usd.Stage.CreateNew(str(path))
    hero = UsdGeom.Xform(stage.OverridePrim("/World/Hero"))
    translate = hero.AddTranslateOp()
    translate.Set(Gf.Vec3d(0.0, 0.0, 0.0), Usd.TimeCode(1))
    translate.Set(Gf.Vec3d(10.0, 0.0, 0.0), Usd.TimeCode(24))
    stage.GetRootLayer().Save()


def build_shot(path: Path) -> None:
    stage = Usd.Stage.CreateNew(str(path))
    root = stage.GetRootLayer()
    root.subLayerPaths = ["./anim.usda", "./layout.usda"]

    stage.SetStartTimeCode(1)
    stage.SetEndTimeCode(24)

    hero = stage.GetPrimAtPath("/World/Hero")
    hero.GetVariantSet("look").SetVariantSelection("muddy")
    stage.GetRootLayer().Save()


def report() -> None:
    shot_path = USD_DIR / "shot.usda"
    stage = Usd.Stage.Open(str(shot_path))
    hero = UsdGeom.Xformable(stage.GetPrimAtPath("/World/Hero"))

    print("LIVRPS: Local (anim) beats layout; Layout references Asset")
    print("Frame 1 translate:", hero.ComputeLocalToWorldTransform(Usd.TimeCode(1)))
    print("Frame 24 translate:", hero.ComputeLocalToWorldTransform(Usd.TimeCode(24)))
    print("Selected look:", stage.GetPrimAtPath("/World/Hero").GetVariantSet("look").GetVariantSelection())


def main() -> None:
    USD_DIR.mkdir(parents=True, exist_ok=True)
    build_asset(USD_DIR / "asset.usda")
    build_layout(USD_DIR / "layout.usda")
    build_anim(USD_DIR / "anim.usda")
    build_shot(USD_DIR / "shot.usda")

    shot = Usd.Stage.Open(str(USD_DIR / "shot.usda"))
    flat = shot.Flatten()
    flat.Export(str(USD_DIR / "shot_flat.usda"))
    report()


if __name__ == "__main__":
    main()
