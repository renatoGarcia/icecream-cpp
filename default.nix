{ pkgs ? import <nixpkgs> {} }:

let
  # There is a bug when regex matching with big strings
  headerSrc = builtins.substring 1000 10000 (builtins.readFile ./icecream.hpp);

  major = builtins.match ".*#define ICECREAM_MAJOR_VERSION[[:space:]]+([[:digit:]]+).*" headerSrc;
  minor = builtins.match ".*#define ICECREAM_MINOR_VERSION[[:space:]]+([[:digit:]]+).*" headerSrc;
  patch = builtins.match ".*#define ICECREAM_PATCH_VERSION[[:space:]]+([[:digit:]]+).*" headerSrc;

  devHash_ = builtins.match ".*#define ICECREAM_DEV_HASH \"([[:xdigit:]]+).*" headerSrc;
  devHash = if devHash_ != null then builtins.elemAt devHash_ 0 else null;

  repo = builtins.fetchGit {
    url = https://github.com/renatoGarcia/icecream-cpp.git;
    ref = "master";
    rev = devHash;
  };

  version = (builtins.concatStringsSep "." (major ++ minor ++ patch))
            + (if devHash != null
               then "+dev." + toString repo.revCount
               else "+local_repo");

in pkgs.stdenv.mkDerivation {
  pname = "icecream-cpp";
  inherit version;
  src = ./.;

  installPhase = ''
    mkdir -p $out/include/
    cp icecream.hpp $out/include
  '';

  meta = with pkgs.stdenv.lib; {
    homepage = "https://github.com/renatoGarcia/icecream-cpp";
    description = "A little library to help with the print debugging on C++11 and forward";
    license = licenses.mit;
    maintainers = with maintainers; [ renatoGarcia ];
  };
}
