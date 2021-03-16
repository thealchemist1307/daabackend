
from django.shortcuts import render

from django.http.response import JsonResponse
from rest_framework.response import Response
from rest_framework.parsers import JSONParser 
from rest_framework import status
 
from coordinates.models import Coordinates
from coordinates.serializers import CoordinateSerializer
from rest_framework.decorators import api_view
import subprocess
from subprocess import Popen, PIPE
import pandas as pd
import csv
from django.http import HttpResponse


@api_view(['GET','POST'])
def handle_coordinates(request):
    if request.method == 'GET':
        coordinate = Coordinates.objects.all()
         
        coordinate_serializer = CoordinateSerializer(coordinate, many=True)
        return JsonResponse(coordinate_serializer.data, safe=False)
    elif request.method == 'POST':
        subprocess.call(["g++", "Test.cc"]) # OR gcc for c program
        p = Popen(['./a.out'], shell=True, stdout=PIPE, stdin=PIPE)

        value = str("2 \n 1 2 3 4 \n 4 5 6 7") + '\n'
        value = bytes(value, 'UTF-8')  # Needed in Python 3.
        p.stdin.write(value)
        p.stdin.flush()
        result = p.stdout.readline().strip()

        with open('rectangles.csv') as csv_file:
            response = HttpResponse(csv_file, content_type='text/csv',status=status.HTTP_201_CREATED)
            filename = "rectangles.csv"
            response['Content-Disposition'] = 'attachment; filename="{}"'.format(filename)

        return response

 
