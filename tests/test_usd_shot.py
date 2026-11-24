from pathlib import Path

import pytest
from pxr import Gf, Usd, UsdGeom


ROOT = Path(__file__).resolve().parents[1]
USD_DIR = ROOT / "build" / "usd"


@pytest.fixture(scope="module", autouse=True)
def build_shot():
    import sys

    sys.path.insert(0, str(ROOT / "tools" / "usd"))
    import build_shot

    build_shot.main()


def test_expected_usda_files_exist():
    for name in ("asset.usda", "layout.usda", "anim.usda", "shot.usda", "shot_flat.usda"):
        assert (USD_DIR / name).is_file()


def test_asset_has_custom_attribute_and_look_variant():
    stage = Usd.Stage.Open(str(USD_DIR / "asset.usda"))
    hero = stage.GetPrimAtPath("/World/Hero")

    assert hero.GetAttribute("creatureKind").Get() == "hydra"
    looks = hero.GetVariantSet("look")
    assert looks.GetVariantNames() == ["clean", "muddy"]


def test_anim_layer_keys_beat_layout_translation():
    stage = Usd.Stage.Open(str(USD_DIR / "shot.usda"))
    hero = UsdGeom.Xformable(stage.GetPrimAtPath("/World/Hero"))

    at_frame_1 = hero.ComputeLocalToWorldTransform(Usd.TimeCode(1)).ExtractTranslation()
    at_frame_24 = hero.ComputeLocalToWorldTransform(Usd.TimeCode(24)).ExtractTranslation()

    assert Gf.IsClose(at_frame_1, Gf.Vec3d(0.0, 0.0, 0.0), 1e-6)
    assert Gf.IsClose(at_frame_24, Gf.Vec3d(10.0, 0.0, 0.0), 1e-6)


def test_shot_selects_muddy_look():
    stage = Usd.Stage.Open(str(USD_DIR / "shot.usda"))
    hero = stage.GetPrimAtPath("/World/Hero")

    assert hero.GetVariantSet("look").GetVariantSelection() == "muddy"
    muddy = UsdGeom.Sphere(hero).GetDisplayColorAttr().Get()
    assert Gf.IsClose(muddy[0], Gf.Vec3f(0.28, 0.18, 0.08), 1e-6)
