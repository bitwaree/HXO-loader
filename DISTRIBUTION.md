# Distribution Terms

## License Overview
This project is distributed under the terms of the [GNU General Public License v3 (GPL-3.0)](https://www.gnu.org/licenses/gpl-3.0.en.html). By using, modifying, or distributing this project, you agree to comply with the terms set forth in this license.

## Additional Conditions for Distribution

1. **Source Code Requirements**:
   - If you distribute copies or derivatives of this project, the source code must be made available under the same license (GPL-3.0).
   - You must provide appropriate credit to the original author (bitware) and maintain all existing copyright notices.
   - Any modifications you make should be clearly documented and attributed.

2. **Binary Distribution**:
   - You are permitted to distribute compiled binaries of this project, provided that the source code is also available under the same conditions.
   - The file `hxo_loader.so` must remain a separate, dynamically loadable library. You must not statically link this file into your binaries unless you comply with the GPL-3.0 terms.

3. **Modifications and Derivative Works**:
   - If you modify the source code or create derivative works, you must make these modifications available under the same license.
   - Clear credit must be given to the original author (bitware) in any modified versions of the project.
   - If you create an injector for a specific process or library using this code, it will be considered a derivative work and must comply with the terms of the GPL-3.0.

4. **Commercial Use**:
   - Commercial use of this project is not permitted if it involves directly profiting from the original codebase. 
   - You may, however, create and sell independent `.hxo` files that are injected by the loader, as long as the loader itself is not modified and proper credit is given.

5. **Forking and Contributions**:
   - Forks of this project are encouraged, provided that they adhere to the same licensing terms.
   - Contributions to the original project are welcome and will be considered under the same license.

## Credits and Attribution

- **inih library**: This project uses the inih library by Ben Hoyt for INI parsing. Proper credit is given to the original author, and it is licensed under the New BSD License.
- **bitware**: The majority of this project is original work by bitware. Please ensure to credit bitware when using or distributing this project.
