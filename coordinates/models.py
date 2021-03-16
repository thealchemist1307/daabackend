
from django.db import models


class Coordinates(models.Model):
    x = models.IntegerField(null=False,default=0)
    y = models.IntegerField(null=False,default=0)
