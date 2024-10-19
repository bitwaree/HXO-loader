# Distribution Terms

## License Overview
The HXO-loader project is licensed primarily under the terms of the [GNU General Public License v3 (GPL-3.0)](https://www.gnu.org/licenses/gpl-3.0.en.html). This ensures that the core functionality of the project remains open-source, free to use, modify, and distribute under the GPL-3.0 license.

For increased flexibility and adaptability, the project also includes distribution terms under the [GNU Lesser General Public License v3 (LGPL-3.0)](https://www.gnu.org/licenses/lgpl-3.0.en.html). This allows users more freedom when dealing with dynamically linking.

Users can choose which license best suits their needs.

## Choosing a License
When using, modifying, or distributing this project, you can choose to comply with either the GPL-3.0 or LGPL-3.0 terms:

- **GPL-3.0**: The project, including all modifications and derivative works, must remain licensed under GPL-3.0. This ensures that all changes to the project and its source code are shared under the same open-source terms.
  
- **LGPL-3.0**: For specific cases where more flexibility is required (e.g., dynamic linking with proprietary code), users can opt to apply the LGPL-3.0 terms to dynamically linked components like `hxo_loader.so` or `libhxo.so`. The LGPL allows combining this library with proprietary software while ensuring the freedom of the library itself.

## Additional Conditions for Distribution

1. **Source Code Requirements**:
   - If you distribute copies or derivatives of this project, the source code must be made available under the same license (either GPL-3.0 or LGPL-3.0, depending on your choice).
   - Any modifications you make should be clearly documented and attributed.

2. **Binary Distribution**:
   - You are permitted to distribute compiled binaries of this project, provided that the source code is also available under the same conditions.
   - The file `hxo_loader.so` or `libhxo.so` must remain a separate, dynamically loadable library. If you choose the LGPL-3.0 license, you may link it with proprietary code, provided the library remains modifiable by the end user.

3. **Modifications and Derivative Works**:
   - If you modify the source code or create derivative works, you must make these modifications available under the same license you choose (either GPL-3.0 or LGPL-3.0).
   - While it's not required, it is recommended that you include a reference to the original project (HXO-loader) and a proper credit to its authors when distributing modified versions. This helps users identify the origins of the project and access the latest official updates.

4. **Commercial Use**:
   - Commercial use of this project is permitted, as long as you comply with the terms of the GPL-3.0 or LGPL-3.0 (depending on your choice).
   - You are free to sell your `.hxo` modules, as long as the source code of this project or dynamically linkable versions of `hxo_loader.so` remain available and users can modify or replace it under the same license.

5. **Forking and Contributions**:
   - Forks of this project are encouraged, provided that they adhere to the same licensing terms.
   - Contributions to the original project are welcome and will be considered under the same license.

## Credits and Attribution

- **inih library**: This project uses the inih library by Ben Hoyt for INI parsing. Proper credit is given to the original author, and it is licensed under the New BSD License.
- **bitware**: The majority of this project is original work by bitware. Please ensure to credit bitware when redistributing this project.
