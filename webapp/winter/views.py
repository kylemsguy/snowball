from django.shortcuts import render
from django.http import *
from django.views.decorators.csrf import *
import json

# Create your views here.

@csrf_exempt
def contextio_callback(request):
	print("We got it ")
	payload = json.loads(str(request.body, 'utf-8'))
	print(payload)
	return HttpResponse("Success")

@csrf_exempt
def contextio_failure_callback(request):
	print("We failed ")
	payload = json.loads(str(request.body, 'utf-8'))
	print(payload)
	return HttpResponse("Success")

def home(request):
	return render(request, 'winter/home.html', {})
