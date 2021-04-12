# Copyright (c) 2020-2021, NVIDIA CORPORATION

import pandas as pd

from geopandas.geoseries import GeoSeries as gpGeoSeries

from typing import (
    TypeVar,
    Union,
)

import cudf
import geopandas as gpd

from cuspatial.io.geopandas_adapter import GeoPandasAdapter
from cuspatial.geometry.geocolumn import GeoColumn, GeoMeta
from cuspatial.geometry.geoarrowbuffers import GeoArrowBuffers


T = TypeVar("T", bound="GeoSeries")


class GeoSeries(cudf.Series):
    """
    cuspatial.GeoSeries enables GPU-backed storage and computation of
    shapely-like objects. Our goal is to give feature parity with GeoPandas.
    At this time, only from_geopandas and to_geopandas are directly supported.
    cuspatial GIS, indexing, and trajectory functions depend on the arrays
    stored in the `GeoArrowBuffers` object, accessible with the `points`,
    `multipoints`, `lines`, and `polygons` accessors.

    >>> cuseries.points
        xy:
        0   -1.0
        1    0.0
        dtype: float64
    """
    def __init__(
        self,
        data: Union[gpd.GeoSeries],
        index: Union[cudf.Index, pd.Index] = None,
        dtype=None,
        name=None,
        nan_as_null=True,
    ):
        # Condition index
        if isinstance(data, (gpGeoSeries, GeoSeries)):
            if index is None:
                index = data.index
        if index is None:
            index = cudf.RangeIndex(0, len(data))
        # Condition data
        if isinstance(data, pd.Series):
            data = gpGeoSeries(data)
        # Create column
        if isinstance(data, GeoColumn):
            column = data
        elif isinstance(data, GeoSeries):
            column = data._column
        elif isinstance(data, gpGeoSeries):
            adapter = GeoPandasAdapter(data)
            buffers = GeoArrowBuffers(adapter.get_geoarrow_host_buffers())
            pandas_meta = GeoMeta(adapter.get_geopandas_meta())
            column = GeoColumn(buffers, pandas_meta)
        else:
            raise TypeError(
                f"Incompatible object passed to GeoSeries ctor {type(data)}"
            )
        super().__init__(column, index, dtype, name, nan_as_null)

    @property
    def geocolumn(self):
        """
        The GeoColumn object keeps a reference to a `GeoArrowBuffers` object,
        which contains all of the geometry coordinates and offsets for thie
        `GeoSeries`.
        """
        return self._column

    @geocolumn.setter
    def geocolumn(self, value):
        if not isinstance(value, GeoColumn):
            raise TypeError
        self._column = value

    @property
    def points(self):
        """
        Access the `PointsArray` of the underlying `GeoArrowBuffers`.
        """
        return self.geocolumn.points

    @property
    def multipoints(self):
        """
        Access the `MultiPointArray` of the underlying `GeoArrowBuffers`.
        """
        return self.geocolumn.multipoints

    @property
    def lines(self):
        """
        Access the `LineArray` of the underlying `GeoArrowBuffers`.
        """
        return self.geocolumn.lines

    @property
    def polygons(self):
        """
        Access the `PolygonArray` of the underlying `GeoArrowBuffers`.
        """
        return self.geocolumn.polygons

    def __getitem__(self, key):
        result = self._column[key]
        return result

    def __repr__(self):
        # TODO: Limit the the number of rows like cudf does
        return self.to_pandas().__repr__()

    def to_geopandas(self, nullable=False):
        """
        Returns a new GeoPandas GeoSeries object from the coordinates in
        the cuspatial GeoSeries.
        """
        if nullable is True:
            raise ValueError("GeoSeries doesn't support <NA> yet")
        host_column = self.geocolumn.to_host()
        output = [geom.to_shapely() for geom in host_column]
        return gpGeoSeries(output, index=self.index.to_pandas())

    def to_pandas(self):
        """
        Treats to_pandas and to_geopandas as the same call, which improves
        compatibility with pandas.
        """
        return self.to_geopandas()
