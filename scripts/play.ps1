if ($args[0] -eq "start") {
    & ./scripts/run.sh deploy 2
    Write-Host "waiting for postgres..."
    Start-Sleep -Seconds 55
    Write-Host "start playing.."
    docker exec -it cnode0 ./server server0 5000 &
    Start-Sleep -Seconds 2
    Get-Content numeri.txt | docker exec -it cnode1 ./client cnode0 5000 &
    #Get-Content numeri.txt | docker exec -i cnode2 ./client cnode0 5000 > $null 2>&1
    #Get-Content numeri.txt | docker exec -i cnode3 ./client cnode0 5000 > $null 2>&1
    Write-Host "done play"


}