clc
clear
%system('rm /HISTO/*.jpeg')
%system('rm/HISTO/*.gif')
for i=1:3
    NOM={'norme' 'vx' 'vy'}
    ADRESSE = '/Network/Servers/helvede.info.espci.fr/home/promo135/tcaroff/Documents/TP_info/Projet/HISTO/';
    filename = [ADRESSE NOM{i} '.dat']
    delimiter=''
    formatSpec = '%f%[^\n\r]';
    fileID = fopen(filename,'r');
    dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter,  'ReturnOnError', false);
    fclose(fileID);
    Hist(:,i+1) = dataArray{:, 1};
    
        figure
    h=histogram(Hist(:, i+1), 30)
    
    saveas(h, [ADRESSE NOM{i} '.jpeg'])
    
    clearvars filename delimiter formatSpec fileID dataArray ans;
end

