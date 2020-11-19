writecommand(TFT_MADCTL);
rotation = m % 4;
switch (rotation)
{
case 0: // Portrait
    writedata(0x08);
    _width  = _init_width;
    _height = _init_height;
    break;
case 1: // Landscape (Portrait + 90)
    writedata(0xA8);
    _width  = _init_height;
    _height = _init_width;
    break;
case 2: // Inverter portrait
    writedata(0x68);
    _width  = _init_width;
    _height = _init_height;
    break;
case 3: // Inverted landscape
    writedata(0xC8);
    _width  = _init_height;
    _height = _init_width;
    break;
}
