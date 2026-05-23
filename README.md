

## Text Rendering APIs

| API                                                 | Parameters                                                                           | Main Difference                         | Rendering Method    | Use Case                    |
| --------------------------------------------------- | ------------------------------------------------------------------------------------ | --------------------------------------- | ------------------- | --------------------------- |
| `printLCD(string str)`                              | `str` → text string                                                                  | Prints complete string directly to LCD  | Immediate rendering | Simple text output          |
| `printToFrame(string text)`                         | `text` → text string                                                                 | Prints string into framebuffer first    | Buffered rendering  | Complex UI rendering        |
| `printletter(char letter, bool flag, int startpos)` | `letter` → character, `flag` → clipping control, `startpos` → starting bitmap column | Low-level single character LCD renderer | Immediate rendering | Internal glyph transmission |
| `printLetterToFrame(char letter)`                   | `letter` → character                                                                 | Character renderer for framebuffer      | Buffered rendering  | Off-screen text composition |
| `marqueetext(string text)`                          | `text` → scrolling text                                                              | Animated scrolling text renderer        | Immediate rendering | Marquee/scroll effects      |

---

## Shape Rendering APIs

| API                                                                | Parameters                                                                  | Main Difference                     | Primitive Type | Use Case                        |
| ------------------------------------------------------------------ | --------------------------------------------------------------------------- | ----------------------------------- | -------------- | ------------------------------- |
| `drawRect(int frame[48][84], int x, int y, int height, int width)` | `frame` → target frame, `x/y` → start position, `height/width` → dimensions | Draws on arbitrary frame array      | Rectangle      | Generic rectangle rendering     |
| `drawRectToFrame(int height, int width)`                           | `height/width` → dimensions                                                 | Draws on attached framebuffer       | Rectangle      | Scene rendering                 |
| `plotLineToFrame(int x, int y)`                                    | `x/y` → destination coordinate                                              | Bresenham incremental rasterization | Line           | Geometry rendering              |
| `plotCircleToFrame(int r)`                                         | `r` → radius                                                                | Midpoint circle rasterization       | Circle         | Efficient circle rendering      |
| `plotTriangleToFrame(int x0, int y0, int x1, int y1)`              | `x0/y0` → point 2, `x1/y1` → point 3                                        | Triangle using composed lines       | Triangle       | Polygon rendering               |
| `point(int x, int y)`                                              | `x/y` → pixel coordinate                                                    | Single pixel write                  | Pixel          | Lowest-level plotting primitive |

---

## Display Pipeline APIs

| API                               | Parameters                                                    | Main Difference                            | Layer      | Use Case                  |
| --------------------------------- | ------------------------------------------------------------- | ------------------------------------------ | ---------- | ------------------------- |
| `display()`                       | None                                                          | Public framebuffer flush API               | High-level | Final scene rendering     |
| `sendframeset(int frame[48][84])` | `frame` → framebuffer pointer                                 | Converts framebuffer into LCD page packets | Mid-level  | Framebuffer serialization |
| `sendDC(int ifdata, string bits)` | `ifdata` → command/data select, `bits` → serialized bitstream | Raw bit-level LCD transmission             | Low-level  | Hardware communication    |

---

## Framebuffer Management APIs

| API                        | Parameters                    | Main Difference                       | Function            | Use Case             |
| -------------------------- | ----------------------------- | ------------------------------------- | ------------------- | -------------------- |
| `attach(int (*frame)[84])` | `frame` → framebuffer pointer | Binds framebuffer to renderer context | Framebuffer binding | Multi-buffer systems |
| `framesetInit()`           | None                          | Initializes framebuffer system        | Initialization      | Rendering setup      |
| `setCursor(int x, int y)`  | `x/y` → cursor position       | Updates renderer cursor position      | State management    | Drawing positioning  |

---

## Coordinate Storage APIs

| API                         | Parameters         | Main Difference                           | Data Flow         | Use Case               |
| --------------------------- | ------------------ | ----------------------------------------- | ----------------- | ---------------------- |
| `CoordsStore(int x, int y)` | `x/y` → coordinate | Stores generated coordinates              | Write operation   | Deferred rasterization |
| `getCoords()`               | None               | Retrieves stored coordinates sequentially | Read operation    | Geometry playback      |
| `destroyStorage()`          | None               | Clears linked-list storage system         | Cleanup operation | Memory reset           |

---

## Hardware Control APIs

| API             | Parameters | Main Difference                        | Hardware Layer        | Use Case           |
| --------------- | ---------- | -------------------------------------- | --------------------- | ------------------ |
| `init()`        | None       | Configures GPIO and LCD reset sequence | Initialization        | Driver startup     |
| `enable()`      | None       | Pulls chip-enable low                  | Communication control | Begin transmission |
| `disable()`     | None       | Pulls chip-enable high                 | Communication control | End transmission   |
| `clearScreen()` | None       | Resets LCD and rendering counters      | Display state control | Full screen clear  |
