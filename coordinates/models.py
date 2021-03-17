
from django.db import models


class Coordinates(models.Model):
    x1 = models.IntegerField(null=False,default=0)
    y1 = models.IntegerField(null=False,default=0)
    x2 = models.IntegerField(null=False,default=0)
    y2 = models.IntegerField(null=False,default=0)
