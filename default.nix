{
  lib,
  stdenv,
  hyprland,
}:
stdenv.mkDerivation {
  pname = "hyprscrollreporter";
  version = "0.1";
  src = ./.;

  inherit (hyprland) nativeBuildInputs;

  buildInputs = [hyprland] ++ hyprland.buildInputs;

  meta = with lib; {
    homepage = "";
    description = "A simple plugin that reports the current workspace transition percentage.";
    license = licenses.bsd3;
    platforms = platforms.linux;
  };
}