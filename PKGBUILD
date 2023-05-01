# AT PROJECT Limited 2023
# by ElCapitan
pkgname=ds
_gitname="Directory-Tree-atdt-"
pkgver=v1.2.6_3
pkgrel=1
arch=('x86_64')
license=('GPL3')
source=("$pkgname-$pkgver.tar.gz::https://github.com/EICapitan/Directory-Tree/archive/refs/tags/atdt-v1.2.6_3.tar.gz")
sha256sums=('SKIP')

build() {
    cd "$_gitname$pkgver"
    cmake .
    make
}

package() {
    cd "$_gitname$pkgver"
    msg2 $pkgdir
    make DESTDIR="$pkgdir/" install
}