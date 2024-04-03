{
  outputs = { nixpkgs, ... }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
      platform = "i686-linux";
      pkgsCross = nixpkgs.legacyPackages.${platform};
    in
    {
      devShells.${system}.default = pkgsCross.stdenv.mkDerivation {
        name = "libffi_shell";
        buildInputs = with pkgsCross; [ libffi ];
      };
    };
}
