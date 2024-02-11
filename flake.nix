{
  inputs =
    {
      nixpkgs.url = "nixpkgs/nixos-unstable";
    };

  outputs = { nixpkgs, ... }:
    let
      system = "x86_64-linux";
      platform = "mingwW64";
      #pkgs = nixpkgs.legacyPackages.${system};
      pkgs = nixpkgs.legacyPackages.${system}.pkgsCross.${platform};
    in
    {
      #nixpkgs.crossSystem = {
      #  config = "x86_64-windows";
      #};
      pkgs.pkgsStatic.callPackage ({ mkShell, bear }: devShells.${system}.default = pkgs.mkShell {
        depsBuildBuild = with pkgs; [
          bear
        ];
      };
    };
}
