#!/bin/env python3

import os
import tempfile
from PIL import Image
from pathlib import Path

local_path = Path(os.path.dirname(os.path.abspath(__file__)))
with tempfile.NamedTemporaryFile(suffix='.jpg') as f:
    # Create image.
    path = local_path / 'random_mandel.py'
    os.system(f'{path} 100 100 {f.name}')

    # Load image and check shape.
    assert Image.open(f.name).size == (100, 100)
